#include "bpt.h"

header_page* hPage; // header page
page_t* rPage; // root page
int fd;

int open_db(char *pathname){
	// success -> file descriptor (>0) , fail -> -1 
	fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0777);
	hPage = (header_page*)calloc(1, PAGESIZE);
	int tmp;

	if (fd > 0){ // DB create
		printf("DB create\n");
		hPage->num_pages = 1;
		tmp = pwrite(fd, hPage, PAGESIZE, SEEK_SET);
		if (tmp < PAGESIZE) {
			printf("write hPage fail\n");
			exit(EXIT_FAILURE);
		}
		return 0;
	}
	
	fd = open(pathname, O_RDWR | O_SYNC);
	if (fd > 0){ // DB open
		printf("DB open!!\n");
		tmp = pread(fd, hPage, PAGESIZE, SEEK_SET);
		
		if (tmp < PAGESIZE) {
			printf("read hPage fail\n");
			exit(EXIT_FAILURE);
		}

		if (hPage->rootPO != 0){
			rPage = (page_t *)calloc(1, PAGESIZE);
			tmp = pread(fd, rPage, PAGESIZE, hPage->rootPO);
			if (tmp < PAGESIZE) {
				printf("read rPage fail\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			printf("empty DB\n");
		}
		return 0;
	}

	printf("fail to open\n");
	return -1;
}

pagenum_t file_alloc_page(){
	//printf("file alloc page...");

	page_t * new_freeP;
	new_freeP = (page_t *)calloc(1, PAGESIZE);
	pagenum_t tmpPN;

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

	pwrite(fd, hPage, PAGESIZE, SEEK_SET);
	free(new_freeP);
	return tmpPN;
}

void file_free_page(pagenum_t pagenum){
	page_t * tmpP;
	tmpP = (page_t *)calloc(1, PAGESIZE);

	tmpP->parentPO = hPage->freePO;
	hPage->freePO = tmpP->parentPO;
	file_write_page(pagenum, tmpP);
	pwrite(fd, hPage, PAGESIZE, SEEK_SET);
	free(tmpP);
}

void file_read_page(pagenum_t pagenum, page_t* dest){
	//printf("file_read_page ... pagenum:%"PRId64"\n",pagenum);
	int readsize = pread(fd, dest, PAGESIZE, pagenum*4096);
	if (readsize < PAGESIZE){
		printf("fail\n");
	}
}

void file_write_page(pagenum_t pagenum, const page_t* src){
	//printf("file_write_page ... pagenum:%"PRId64"\n",pagenum);
	int writesize = pwrite(fd, src, PAGESIZE, pagenum*4096);
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
}

char * find(int64_t key){
	page_t * fP;
	//printf("find(%ld)\n",key);
	fP = (page_t *)calloc(1, PAGESIZE);
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

pagenum_t find_leaf(page_t * fP, int64_t key){
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
int insert(int64_t key, char * value){

	// no tree -> start new tree
	if (rPage == NULL) {
		//printf(" ... start new tree\n");
		return start_new_tree(key, value);
	}

	// tree exists

	if (find(key) != NULL) {
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
		return insert_into_leaf(leafnum, key, value);
	}

		// leaf split
	//printf("leaf split\n");
	free(leaf);
	return insert_into_leaf_after_splitting(leafnum, key, value);
}

int start_new_tree(int64_t key, char * value){
	rPage = (page_t *)calloc(1, PAGESIZE);
	rPage = make_leaf(rPage);

	rPage->records[0].key = key;
	strcpy(rPage->records[0].value, value);
	rPage->parentPO = 0;
	rPage->extraPO = 0;
	rPage->num_keys++;

	pagenum_t rPN = file_alloc_page();
	file_write_page(rPN, rPage);
	hPage->rootPO = rPN*4096;
	pwrite(fd, hPage, PAGESIZE, SEEK_SET);
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

int insert_into_leaf(pagenum_t leafnum, int64_t key, char * value){
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

int insert_into_leaf_after_splitting(pagenum_t leafnum, int64_t key, char * value){
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

   pagenum_t new_leafnum = file_alloc_page();
   file_write_page(new_leafnum, new_leaf);

   leaf->extraPO = new_leafnum*4096;

   file_write_page(leafnum, leaf);

   free(leaf);
   free(new_leaf);

   return insert_into_parent(leafnum, new_key, new_leafnum);
}

int insert_into_parent(pagenum_t left, int64_t key, pagenum_t right){	
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
		return insert_into_new_root(left, key, right);
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
		return insert_into_node(parentPN, ins_idx, key, right);
	}

	// node split
	return insert_into_node_after_splitting(parentPN, ins_idx, key, right);
}

int insert_into_new_root(pagenum_t left, int64_t key, pagenum_t right){
	page_t * root = make_node(root);
	pagenum_t rootPN = file_alloc_page();

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
	hPage->rootPO = rootPN*4096;
	int writesize = pwrite(fd, hPage, PAGESIZE, SEEK_SET);

	file_read_page(rootPN, rPage);

	free(root);
	if (writesize < PAGESIZE) return -1;
	else return 0;
}

int insert_into_node(pagenum_t parent, int ins_idx, int64_t key, pagenum_t right){
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

int insert_into_node_after_splitting(pagenum_t parent, int ins_idx, int64_t key, pagenum_t right){
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
		pagenum_t newPN = file_alloc_page();
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

		return insert_into_parent(parent, k_prime, newPN);
}
