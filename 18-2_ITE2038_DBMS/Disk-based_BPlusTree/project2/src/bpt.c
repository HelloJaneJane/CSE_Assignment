#include "bpt.h"

buffer_manager* buf_man;

int now_tid;

int init_db(int num_buf){
	buf_man->capacity = num_buf;

	buffer_structure * init_buf = (buffer_structure*)malloc(sizeof(buffer_structure));
	init_buf->table_id = 0;
	init_buf->page_num = 0;
	init_buf->isDirty = false;
	init_buf->pin_cnt = 0;
	init_buf->nxtLRU = 0;
	init_buf->prvLRU = 0;
	buf_man->buffer_pool = init_buf;
	free(init_buf);

	return 0;
}

int open_table(char *pathname){
	// success -> file descriptor (>0) , fail -> -1 
	int fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0777);
	header_page * hPage = (header_page*)calloc(1, PAGESIZE);
	int tmp;

	if (fd > 0){ // table create
		printf("table create\n");
		hPage->num_pages = 1;
		tmp = pwrite(fd, hPage, PAGESIZE, SEEK_SET);
		if (tmp < PAGESIZE) {
			printf("write hPage fail\n");
			exit(EXIT_FAILURE);
		}
		return now_tid = fd;
	}
	
	fd = open(pathname, O_RDWR | O_SYNC);
	if (fd > 0){ // table open
		printf("table open!!\n");
		tmp = pread(fd, hPage, PAGESIZE, SEEK_SET);
		
		if (tmp < PAGESIZE) {
			printf("read hPage fail\n");
			exit(EXIT_FAILURE);
		}

		if (hPage->rootPO != 0){
			page_t * rPage = (page_t *)calloc(1, PAGESIZE);
			tmp = pread(fd, rPage, PAGESIZE, hPage->rootPO);
			free(rPage);
			if (tmp < PAGESIZE) {
				printf("read rPage fail\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			printf("empty table\n");
		}
		return now_tid = fd;
	}

	printf("fail to open\n");
	return -1;
}

int close_table(int table_id){
}

int shutdown_db(){
	buf_man = NULL;
	if(buf_man == NULL) return 0;
	else return -1;
}

pagenum_t file_alloc_page(int table_id){
	//printf("file alloc page...");

	page_t * new_freeP;
	new_freeP = (page_t *)calloc(1, PAGESIZE);
	pagenum_t tmpPN;

	header_page* hPage = buf_man->buffer_pool->hPage;

	if (hPage->freePO != 0) { // free page exists
		//printf("free page exists -> file_read_page\n");
		tmpPN = hPage->freePO/4096;
		file_read_page(tmpPN, new_freeP);
		hPage->freePO = new_freeP->parentPO;
		new_freeP->parentPO = 0;
	}
	else { // make free page
		//printf("free page not exists -> new free page -> file_write_page\n");
		tmpPN = hPage->num_pages;
		file_write_page(tmpPN, new_freeP);
		(hPage->num_pages)++;
	}

	pwrite(table_id, hPage, PAGESIZE, SEEK_SET);
	free(new_freeP);
	free(hPage);
	return tmpPN;
}

void file_free_page(pagenum_t pagenum){
	page_t * tmpP;
	tmpP = (page_t *)calloc(1, PAGESIZE);

	header_page* hPage = buf_man->buffer_pool->hPage;

	tmpP->parentPO = hPage->freePO;
	hPage->freePO = tmpP->parentPO;
	file_write_page(pagenum, tmpP);
	pwrite(now_tid, hPage, PAGESIZE, SEEK_SET);
	free(tmpP);
	free(hPage);
}

void file_read_page(pagenum_t pagenum, page_t* dest){
	//printf("file_read_page ... pagenum:%"PRId64"\n",pagenum);
	int readsize = pread(now_tid, dest, PAGESIZE, pagenum*4096);
	if (readsize < PAGESIZE){
		printf("fail\n");
	}
}

void file_write_page(pagenum_t pagenum, const page_t* src){
	//printf("file_write_page ... pagenum:%"PRId64"\n",pagenum);
	int writesize = pwrite(now_tid, src, PAGESIZE, pagenum*4096);
	if (writesize < PAGESIZE){
		printf("fail\n");
	}
}

void usage( void ) {
	printf("***** 2017030137 Jane Yi b+tree *****\n\n");
	printf("Enter any of the following commands after the prompt > :\n"
    "\ti <k> <v>  -- Insert <k> (int) as key and <v> (char *) as value).\n"
    "\tf <k>  -- Find the value under key <k>.\n"
    "\td <k>  -- Delete key <k> and its associated value.\n"
    "\tt -- Print the B+ tree.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n");
}

queue * c;
queue * eq;

void enqueue(off_t newPO){
	//printf("  enqueue!\n");

	c = (queue *)malloc(sizeof(c));
	eq = (queue *)malloc(sizeof(eq));
	c->nowPO = newPO;
	c->nxt = NULL;
	if (qu == NULL){
		//printf("(qu==NULL)\n");
		qu = c;
	}
	else {
		//printf("(qu!=NULL)\n");
		eq = qu;
		while(eq->nxt != NULL) eq = eq->nxt;
		eq->nxt = c;	
	}
}

queue * dq;

off_t dequeue( void ){
	//printf("  dequeue!\n");
	dq = (queue *)malloc(sizeof(dq));
	dq = qu;
	qu = qu->nxt;

	if (qu == NULL) { // dequeue finish
		free(qu);
	}

	off_t save = dq->nowPO;
	return save;
}

int path_to_root( off_t rootPO, off_t childPO){
	int length = 0;
	off_t c = childPO;
	page_t * cP;
	file_read_page(childPO/4096, cP);
	while (c != rootPO){
		c = cP->parentPO;
		length++;
	}
	return length;
	free(cP);
}

void print_tree(){
	page_t * tmpP;
	off_t tmpO;
	int i=0;

	header_page* hPage = buf_man->buffer_pool->hPage;
	
	if (hPage->rootPO == 0) {
		printf("Empty tree.\n");
		return;
	}

    tmpP = (page_t *)calloc(1, PAGESIZE);
    qu = (queue*)malloc(sizeof(queue));
    qu = NULL;
    enqueue(hPage->rootPO);
    while ( qu != NULL ) {
        tmpO = dequeue();
        file_read_page(tmpO/4096, tmpP);

		if(tmpO != hPage->rootPO){
			page_t * tmpPP = (page_t *)calloc(1, PAGESIZE);
			file_read_page(tmpP->parentPO/4096, tmpPP);

			if(tmpPP->extraPO == tmpO) printf("\n");
		}

        for ( i = 0; i < tmpP->num_keys; ++i ) {
            printf("%"PRId64" ", tmpP->is_leaf ? tmpP->records[i].key : tmpP->entries[i].key);
        }
        if ( !tmpP->is_leaf ) {
            enqueue(tmpP->extraPO);
            for ( i = 0; i < tmpP->num_keys; ++i )
                enqueue(tmpP->entries[i].entPO);
        }
        printf("| ");
    }
    printf("\n");

    free(tmpP);
    free(qu);

	free(eq);
	free(dq);

	free(hPage);
}

char * find(int table_id, int64_t key){
	page_t * fP;
	//printf("find(%ld)\n",key);
	fP = (page_t *)calloc(1, PAGESIZE);

	header_page* hPage = buf_man->buffer_pool->hPage;
	file_read_page(hPage->rootPO/4096, fP);

	pagenum_t fPN = find_leaf(fP, key);
	if (fPN == 0) return NULL;

	char * ans;

	//if(fP == NULL) return NULL;
	int i=0;
	for (i=0; i< fP->num_keys; i++){
		if (fP->records[i].key == key) break;
	}

	if (i == fP->num_keys) {
		ans = NULL;
	}
	else ans = fP->records[i].value; 

	//printf("ans:%s\n",ans);

	free(fP);
	return ans;
}

pagenum_t find_leaf(page_t* fP, int64_t key){
	header_page* hPage = buf_man->buffer_pool->hPage;
	off_t newPO = hPage->rootPO; // start from root
	file_read_page(newPO/4096, fP);

	if (fP == NULL) {
		printf("Empty tree\n");
		return 0;
	}
	int i = 0;
	//int cnt = 1;
	while(!fP->is_leaf){
		//printf("   cnt:%d\n",cnt++);
		for (i=0; i<fP->num_keys; i++){	
			if (i==0 && key < fP->entries[i].key) {
				newPO = fP->extraPO;
				break;
			}
			if (key >= fP->entries[i].key){
				newPO = fP->entries[i].entPO;
				if (i == fP->num_keys-1) break;
				continue;
			}
			else {
				newPO = fP->entries[i-1].entPO;
				break;
			}
		} 
		if (i == fP->num_keys) return 0;
		file_read_page(newPO/4096, fP);
	}
	return newPO/4096;
}

int cut(int length){
	if (length % 2 == 0) return length/2;
	else return length/2 +1;
}

// insert success -> return  0 / fail -> return -1
int insert(int table_id, int64_t key, char * value){
	header_page* hPage = buf_man->buffer_pool->hPage;
	
	// no tree -> start new tree
	if (hPage->rootPO == 0) {
		//printf(" ... start new tree\n");
		return start_new_tree(table_id, key, value);
	}

	// tree exists

	if (find(table_id, key) != NULL) {
		printf("already exists key\n");
		return -1; // insert fail
	}

	page_t * leaf = (page_t *)calloc(1, PAGESIZE);
	pagenum_t leafnum = find_leaf(leaf, key);
	//printf("leafnum:%"PRId64"\n",leafnum);
	
		// leaf has room -> insert into leaf -> success
	if (leaf->num_keys < record_order - 1){
		//printf("insert into leaf\n");
		free(leaf);
		return insert_into_leaf(table_id, leafnum, key, value);
	}

		// leaf split
	//printf("leaf split\n");
	free(leaf);
	return insert_into_leaf_after_splitting(table_id, leafnum, key, value);
}

int start_new_tree(int table_id, int64_t key, char * value){
	header_page* hPage = buf_man->buffer_pool->hPage;
	page_t * rPage = (page_t *)calloc(1, PAGESIZE);
	rPage = make_leaf(rPage);

	rPage->records[0].key = key;
	strcpy(rPage->records[0].value, value);
	rPage->parentPO = 0;
	rPage->extraPO = 0;
	rPage->num_keys++;

	pagenum_t rPN = file_alloc_page(table_id);
	file_write_page(rPN, rPage);
	hPage->rootPO = rPN*4096;
	pwrite(table_id, hPage, PAGESIZE, SEEK_SET);

	free(rPage);
	return 0;
}

page_t * make_leaf(page_t * leaf){
	leaf = make_node(leaf);
	leaf->is_leaf = true;
	return leaf;
}

page_t * make_node(page_t * new_node){
	new_node = (page_t *)calloc(1, PAGESIZE);
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parentPO = 0;
	new_node->extraPO = 0;
	return new_node;
}

int insert_into_leaf(int table_id, pagenum_t leafnum, int64_t key, char * value){
	//printf(" !! insert into leaf\n");

	page_t * leaf = (page_t *)calloc(1, PAGESIZE);
	file_read_page(leafnum, leaf);

	int ins_pt = 0;

	while (ins_pt < leaf->num_keys && leaf->records[ins_pt].key < key) ins_pt++;

	for (int i = leaf->num_keys; i > ins_pt; i--){
		leaf->records[i].key = leaf->records[i-1].key;
		strcpy(leaf->records[i].value, leaf->records[i-1].value);
	}
	leaf->records[ins_pt].key = key;
	strcpy(leaf->records[ins_pt].value, value);
	leaf->num_keys++;

	file_write_page(leafnum, leaf);
	free(leaf);
	return 0;
}

int insert_into_leaf_after_splitting(int table_id, pagenum_t leafnum, int64_t key, char * value){
	//printf(" !! insert into leaf after splitting\n");
	page_t * leaf = (page_t *)calloc(1, PAGESIZE);
	file_read_page(leafnum, leaf);
	page_t * new_leaf = make_leaf(new_leaf);

   int64_t tmp_keys[record_order + 1];
   char tmp_values[record_order + 1][120];

   int ins_idx = 0;

   while (ins_idx < record_order -1 && leaf->records[ins_idx].key < key) ins_idx++;
   //printf("ins_idx :%d\n",ins_idx);

   int i,j;
   for(i=0, j=0; i < leaf->num_keys; i++, j++){
      if (j==ins_idx) j++;
      tmp_keys[j] = leaf->records[i].key;
      strcpy(tmp_values[j], leaf->records[i].value);
   }

   tmp_keys[ins_idx] = key;
   strcpy(tmp_values[ins_idx], value);

   leaf->num_keys = 0;

   int split = cut(record_order -1);
   //printf("split:%d\n",split);

   for(i=0; i<split; i++){
      leaf->records[i].key = tmp_keys[i];
      strcpy(leaf->records[i].value, tmp_values[i]);
      leaf->num_keys++;
   }
   
   for(i=split, j=0; i < record_order; i++, j++){
      new_leaf->records[j].key = tmp_keys[i];
      strcpy(new_leaf->records[j].value, tmp_values[i]);
      new_leaf->num_keys++;
   }

   new_leaf->parentPO = leaf->parentPO;
   //printf("new_leaf->parentPO = leaf->parentPO : %ld\n",leaf->parentPO);

   int64_t new_key = new_leaf->records[0].key;

   pagenum_t new_leafnum = file_alloc_page(table_id);
   file_write_page(new_leafnum, new_leaf);

   leaf->extraPO = new_leafnum*4096;

   file_write_page(leafnum, leaf);

   free(leaf);
   free(new_leaf);

   return insert_into_parent(table_id, leafnum, new_key, new_leafnum);
}

int insert_into_parent(int table_id, pagenum_t left, int64_t key, pagenum_t right){	
	//printf(" !! insert into parent\n");

	page_t * leftP = (page_t *)calloc(1, PAGESIZE);
	file_read_page(left, leftP);
	page_t * parentP = (page_t *)calloc(1, PAGESIZE);
	pagenum_t parentPN = leftP->parentPO/4096;
	file_read_page(parentPN, parentP);

	free(leftP);

	// new root
	if	(parentPN == 0){
		//printf("parent NULL -> new root\n");
		free(parentP);
		return insert_into_new_root(table_id, left, key, right);
	}

	int ins_idx=0;
	for(ins_idx=0; ins_idx<parentP->num_keys; ins_idx++){
		if (parentP->entries[ins_idx].entPO == left*4096) break;
	}
	ins_idx++;

	int parentNK = parentP->num_keys;

	free(parentP);

	// new key fit
	if (parentNK < entry_order - 1){
		//printf("key fit in parent -> insert into node\n");
		return insert_into_node(table_id, parentPN, ins_idx, key, right);
	}

	// node split
	return insert_into_node_after_splitting(table_id, parentPN, ins_idx, key, right);
}

int insert_into_new_root(int table_id, pagenum_t left, int64_t key, pagenum_t right){
	page_t * root = make_node(root);
	pagenum_t rootPN = file_alloc_page(table_id);

	root->entries[0].key = key;
	root->num_keys++;
	root->parentPO = 0;

	root->extraPO = left*4096;
	root->entries[0].entPO = right*4096;

	page_t * leftP = (page_t *)calloc(1, PAGESIZE);
	file_read_page(left, leftP);
	leftP->parentPO = rootPN*4096;
	file_write_page(left, leftP);
	free(leftP);

	page_t * rightP = (page_t *)calloc(1, PAGESIZE);
	file_read_page(right, rightP);
	rightP->parentPO = rootPN*4096;
	file_write_page(right, rightP);
	free(rightP);

	file_write_page(rootPN, root);

	header_page* hPage = buf_man->buffer_pool->hPage;
	hPage->rootPO = rootPN*4096;
	int writesize = pwrite(now_tid, hPage, PAGESIZE, SEEK_SET);

	free(root);
	if (writesize < PAGESIZE) return -1;
	else return 0;
}

int insert_into_node(int table_id, pagenum_t parent, int ins_idx, int64_t key, pagenum_t right){
	//printf(" !! insert into node ... ins_idx:%d, key:%ld, right:%ld\n",ins_idx,key,right);

	page_t * p = (page_t *)calloc(1, PAGESIZE);
	file_read_page(parent, p);

	for(int i=p->num_keys; i>ins_idx; i--){
		p->entries[i].entPO = p->entries[i-1].entPO;
		p->entries[i].key = p->entries[i-1].key;
	}
	p->entries[ins_idx].entPO = right*4096;
	p->entries[ins_idx].key = key;
	p->num_keys++;

	file_write_page(parent, p);

	free(p);
	return 0;
}

int insert_into_node_after_splitting(int table_id, pagenum_t parent, int ins_idx, int64_t key, pagenum_t right){
		//printf(" !! insert into node after splitting\n");

		// save to temp from old
		page_t * old_node = (page_t *)calloc(1, PAGESIZE);
		file_read_page(parent, old_node);
		
		int64_t tmp_keys[entry_order];
		off_t tmp_offset[entry_order + 1];

		tmp_offset[0] = old_node->extraPO;
		for(int i=0,j=1; i<old_node->num_keys+1; i++,j++){
			if (j == ins_idx + 1) j++;
			tmp_offset[j] = old_node->entries[i].entPO;
		}
		for(int i=0,j=0; i<old_node->num_keys; i++,j++){
			if (j == ins_idx) j++;
			tmp_keys[j] = old_node->entries[i].key;
		}
		tmp_offset[ins_idx+1] = right*4096;
		tmp_keys[ins_idx] = key;

		// create new & copy half to old, half to new
		page_t * new_node = make_node(new_node);
		pagenum_t newPN = file_alloc_page(table_id);
		int split = cut(entry_order);
		old_node->num_keys = 0;

		int i, j;
		old_node->extraPO = tmp_offset[0];
		for(i=0; i<split-1; i++){
			old_node->entries[i].entPO = tmp_offset[i+1];
			old_node->entries[i].key = tmp_keys[i];
			old_node->num_keys++;
		}
		int k_prime = tmp_keys[split-1];
		new_node->extraPO = tmp_offset[split];
		for(i=split, j=0; i<entry_order; i++, j++){
			new_node->entries[j].entPO = tmp_offset[i+1];
			new_node->entries[j].key = tmp_keys[i];
			new_node->num_keys++;
		}
		
		// connect parent & child
		new_node->parentPO = old_node->parentPO;
		
		page_t * childP = (page_t *)calloc(1, PAGESIZE);
		for (int i=0; i<new_node->num_keys; i++){
			off_t tmpO = new_node->entries[i].entPO;
			file_read_page(tmpO/4096, childP);
			childP->parentPO = newPN*4096;
		}
		free(childP);		
		

		// finish

		file_write_page(parent, old_node);
		file_write_page(newPN, new_node);

		free(old_node);
		free(new_node);

		return insert_into_parent(table_id, parent, k_prime, newPN);
}

int delete (int table_id, int64_t key){
	char * val = NULL;
	val = find(table_id, key);
	if (val == NULL) {
		free(val);
		return -1;
	}

	page_t * leafP;
	pagenum_t leafPN = find_leaf(leafP, key);

	delete_entry(table_id, leafPN, key);

	return 0;
}

void delete_entry(int table_id, pagenum_t nodePN, int64_t key){
	page_t * nodeP;
	file_read_page(nodePN, nodeP);
	page_t * parentP;
	file_read_page(nodeP->parentPO/4096, parentP);

	// remove key from node
	remove_entry_from_node(table_id, nodeP, key);

	header_page* hPage = buf_man->buffer_pool->hPage;
	// case : deletion from root
	if (nodePN*4096 == hPage->rootPO) {
		adjust_root(table_id);
		free(nodeP);
		free(parentP);
		return;
	}

	// case : deletion from node below root
	
	int min_keys = 1; // p->is_leaf ? cut(record_order-1) : cut(entry_order)-1;
	// delayed merge 니까

		// node stays at or above minimum
	if (nodeP->num_keys >= min_keys) return;

		// node falls beloe minimum (coalescence / redistribution)
	int neighbor_idx = get_neighbor_index(table_id, nodeP);
	int k_prime_idx = (neighbor_idx == -1 ? 0 : neighbor_idx);
	int k_prime = parentP->entries[k_prime_idx].key;
	off_t neighborO = (neighbor_idx == -1 ? 
		parentP->entries[1].entPO : parentP->entries[neighbor_idx].entPO);

	int capacity = (nodeP->is_leaf ? record_order : entry_order-1);

	page_t * neighborP;
	file_read_page(neighborO/4096, neighborP);
	int tmp = neighborP->num_keys + nodeP->num_keys;

	free(nodeP);
	free(parentP);
	free(neighborP);

	if(tmp < capacity) return coalesce_nodes(table_id, nodePN, neighborO/4096, neighbor_idx, k_prime);
	else return redistribute_nodes(table_id, nodePN, neighborO/4096, neighbor_idx, k_prime_idx, k_prime);

}

void remove_entry_from_node(int table_id, page_t * nodeP, int64_t key){
	if(nodeP->is_leaf){ // leaf
		int i=0;
		while (nodeP->records[i].key != key) i++;
		for(++i; i<nodeP->num_keys; i++) {
			nodeP->records[i-1].key = nodeP->records[i].key;
			strcpy(nodeP->records[i-1].value, nodeP->records[i].value);
		}
		nodeP->num_keys--;
	}
	else { // internal
		int i=0;
		while (nodeP->entries[i].key != key) i++;
		for(++i; i<nodeP->num_keys; i++) {
			nodeP->entries[i-1].key = nodeP->entries[i].key;
			nodeP->entries[i-1].entPO = nodeP->entries[i].entPO;
		}
		nodeP->num_keys--;
	}
}

int get_neighbor_index(int table_id, pagenum_t nodePN){
	page_t * nodeP;
	file_read_page(nodePN, nodeP);
	page_t * parentP;
	file_read_page((nodeP->parentPO)/4096, parentP);
	for(int i=0; i<=parentP->num_keys; i++){
		if (parentP->entries[i].entPO == nodePN*4096) return i-1;
	}

	// error
	return -1;
}

void adjust_root(int table_id){
	header_page* hPage = buf_man->buffer_pool->hPage;
	page_t * rPage;
	file_read_page(hPage->rootPO/4096, rPage);
	
	int old_rootPN = hPage->rootPO/4096;

	// nonempty root
	if(rPage->num_keys > 0) return;

	// empty root
	page_t * new_rPage;
		// child exist -> first child as new root
	if(!rPage->is_leaf){
		hPage->rootPO = rPage->extraPO;
		file_read_page(rPage->extraPO/4096, new_rPage);
		new_rPage->parentPO = 0;
		file_write_page(hPage->rootPO/4096, new_rPage);
		pwrite(now_tid, hPage, PAGESIZE, SEEK_SET);
	}
		// no child -> whole tree empty
	else {
		hPage->rootPO = 0;
		pwrite(now_tid, hPage, PAGESIZE, SEEK_SET);
	}

	file_free_page(old_rootPN);
	free(new_rPage);
	free(rPage);
}
page_t * coalesce_nodes(int table_id, pagenum_t nodePN, pagenum_t neighborPN, int neighbor_idx, int k_prime){

	// extreme left -> swap
	if(neighbor_idx == -1){
		pagenum_t tmp = nodePN;
		nodePN = neighborPN;
		neighborPN = tmp;
	}

	page_t * nodeP;
	file_read_page(nodePN, nodeP);

	page_t * neighborP;
	file_read_page(neighborPN, neighborP);

	int neighbor_insert_idx = neighborP->num_keys;

	// nonleaf node
	if (!nodeP->is_leaf){
		neighborP->entries[neighbor_insert_idx].key = k_prime;
		neighborP->num_keys++;

		int n_end = nodeP->num_keys;
		int i,j;

		for(i=neighbor_insert_idx+1, j=0; j<n_end; i++, j++){
			neighborP->entries[i].key = nodeP->entries[j].key;
			neighborP->entries[i].entPO = nodeP->entries[j].entPO;
			neighborP->num_keys++;
			nodeP->num_keys--;
		}
		neighborP->entries[i].entPO = nodeP->entries[j].entPO;

		for(i=0; i<neighborP->num_keys+1; i++){
			page_t * childP;
			file_read_page(neighborP->entries[i].entPO/4096, childP);
			childP->parentPO = neighborPN*4096;
			file_write_page(neighborP->entries[i].entPO/4096, childP);
		}
		file_write_page(neighborPN, neighborP);
		file_free_page(nodePN);
	}
	// leaf
	else {
		for(int i=neighbor_insert_idx, j=0; j<nodeP->num_keys; i++, j++){
			neighborP->records[i].key = nodeP->records[j].key;
			strcpy(neighborP->records[i].value, nodeP->records[j].value);
			neighborP->num_keys++;
		}
		neighborP->extraPO = nodeP->extraPO;
		file_write_page(neighborPN, neighborP);
		file_free_page(nodePN);
	}

	delete_entry(table_id, nodeP->parentPO/4096, k_prime);

	free(nodeP);
	free(neighborP);
}
page_t * redistribute_nodes(int table_id, pagenum_t nodePN, pagenum_t neighborPN, int neighbor_idx, int k_prime_idx, int k_prime){
	
	page_t * nodeP;
	file_read_page(nodePN, nodeP);

	page_t * neighborP;
	file_read_page(neighborPN, neighborP);

	// neighbor to left
	if(neighbor_idx!=-1){
		if(!nodeP->is_leaf){ // internal
			nodeP->entries[nodeP->num_keys+1].entPO = nodeP->entries[nodeP->num_keys].entPO;
			for(int i=nodeP->num_keys; i>0; i--){
				nodeP->entries[i].key = nodeP->entries[i-1].key;
				nodeP->entries[i].entPO = nodeP->entries[i-1].entPO;
			}
			nodeP->entries[0].entPO = neighborP->entries[neighborP->num_keys].entPO;

			page_t * childP;
			file_read_page(nodeP->entries[0].entPO/4096, childP);
			childP->parentPO = nodePN*4096;
			file_write_page(nodeP->entries[0].entPO/4096, childP);
			free(childP);

			neighborP->entries[neighborP->num_keys].entPO = 0;
			nodeP->entries[0].key = k_prime;

			page_t * parentP;
			file_read_page(nodeP->parentPO/4096, parentP);
			parentP->entries[k_prime_idx].key = neighborP->entries[neighborP->num_keys-1].key;
			file_write_page(nodeP->parentPO/4096, parentP);
			free(parentP);

			nodeP->num_keys++;
			neighborP->num_keys--;

			file_write_page(nodePN, nodeP);
			file_write_page(neighborPN, neighborP);
		}
		else{ // leaf
			for(int i=nodeP->num_keys; i>0; i--){
				nodeP->records[i].key = nodeP->records[i-1].key;
				strcpy(nodeP->records[i].value, nodeP->records[i-1].value);
			}
			strcpy(nodeP->records[0].value, neighborP->records[neighborP->num_keys -1].value);
			strcpy(neighborP->records[neighborP->num_keys -1].value, NULL);
			nodeP->records[0].key = neighborP->records[neighborP->num_keys -1].key;

			page_t * parentP;
			file_read_page(nodeP->parentPO/4096, parentP);
			parentP->entries[k_prime_idx].key = nodeP->records[0].key;
			file_write_page(nodeP->parentPO/4096, parentP);
			free(parentP);

			nodeP->num_keys++;
			neighborP->num_keys--;
			
			file_write_page(nodePN, nodeP);
			file_write_page(neighborPN, neighborP);
		}
	}
	// leftmost child
	else{
		if(nodeP->is_leaf){ // leaf
			nodeP->records[nodeP->num_keys].key = neighborP->records[0].key;
			strcpy(nodeP->records[nodeP->num_keys].value, neighborP->records[0].value);

			page_t * parentP;
			file_read_page(nodeP->parentPO/4096, parentP);
			parentP->entries[k_prime_idx].key = neighborP->records[1].key;
			file_write_page(nodeP->parentPO/4096, parentP);
			free(parentP);

			for(int i=0; i<neighborP->num_keys-1; i++){
				neighborP->records[i].key = neighborP->records[i+1].key;
				strcpy(neighborP->records[i].value, neighborP->records[i+1].value);
			}

			nodeP->num_keys++;
			neighborP->num_keys--;

			file_write_page(nodePN, nodeP);
			file_write_page(neighborPN, neighborP);
		}
		else{ // internal
			nodeP->entries[nodeP->num_keys].key = k_prime;
			nodeP->entries[nodeP->num_keys+1].entPO = neighborP->entries[0].entPO;
			
			page_t * childP;
			file_read_page(nodeP->entries[nodeP->num_keys +1].entPO/4096, childP);
			childP->parentPO = nodePN*4096;
			file_write_page(nodeP->entries[nodeP->num_keys +1].entPO/4096, childP);
			free(childP);

			page_t * parentP;
			file_read_page(nodeP->parentPO/4096, parentP);
			parentP->entries[k_prime_idx].key = neighborP->entries[0].key;
			file_write_page(nodeP->parentPO/4096, parentP);
			free(parentP);

			int i;
			for(i=0; i<neighborP->num_keys -1; i++){
				neighborP->entries[i].key = neighborP->entries[i+1].key;
				neighborP->entries[i].entPO = neighborP->entries[i+1].entPO;
			}
			neighborP->entries[i].entPO = neighborP->entries[i+1].entPO;
			
			file_write_page(nodePN, nodeP);
			file_write_page(neighborPN, neighborP);
		}
	}
	free(nodeP);
	free(neighborP);
}
