#include "bpt.h"

buffer_manager* buf_man;

int init_db(int num_buf){
	buf_man = (buffer_manager*)malloc(sizeof(buffer_manager));
	buf_man->capacity = num_buf;

	for(int i=0; i<BUFSIZE; i++) table[i] = -1;

	return 0;
}

int open_table(char *pathname, int num_column){
	// success -> file descriptor (>0) , fail -> -1 
	int fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0777);
	header_page* hPage = (header_page*)calloc(1, PAGESIZE);
	int tmp;

	if (fd > 0){ // table not exists -> create
		printf("table create\n");
		hPage->num_pages = 1;
		hPage->freePO = 0;
		hPage->rootPO = 0;
		hPage->num_columns = num_column;
		tmp = pwrite(fd, hPage, PAGESIZE, SEEK_SET);
		if (tmp < PAGESIZE) {
			printf("write hPage fail\n");
			return -1;
		}
	}
	
	// table exists
	else{
		fd = open(pathname, O_RDWR | O_SYNC);
		if (fd > 0){ // table open
			printf("table open!!\n");
			tmp = pread(fd, hPage, PAGESIZE, SEEK_SET);
			if (tmp < PAGESIZE) {
				printf("read hPage fail\n");
				return -1;
			}
		}
		else {
			printf("fail to open\n");
			return -1;
		}
	}

	int i;
	for(i=0; i<BUFSIZE; i++){
		if (table[i]<=0) break;
	}
	if (i==BUFSIZE){
		printf("full table\n");
		return -1;
	}
	table[i] = fd;
	buf_put_page(fd, 0);
	buf_read_page(fd, 0);
	free(hPage);
	return fd;
}

int close_table(int table_id){
	int i;
	for(i=0; i<BUFSIZE; i++){
		if (table[i] == table_id) break;
	}
	if (i==BUFSIZE) return -1;

	buffer_structure* buf_hPage = buf_read_page(table_id, 0);
	buffer_structure* tmp_buf = buf_man->LRUidx;

	while(tmp_buf!=NULL){
		if (tmp_buf->table_id==table_id && tmp_buf->page_num!=0)
			buf_free_page(tmp_buf);
		tmp_buf = tmp_buf->nxtLRU;
	}
	pwrite(table_id, buf_hPage->hPage, PAGESIZE, SEEK_SET);

	// next exists -> nxt's prv = prev
	if(buf_hPage->nxtLRU != NULL) buf_hPage->nxtLRU->prvLRU = buf_hPage->prvLRU;
	
	// prev exists -> prv's nxt = next
	if(buf_hPage->prvLRU != NULL) buf_hPage->prvLRU->nxtLRU = buf_hPage->nxtLRU;
	else {
		buf_man->LRUidx = buf_hPage->nxtLRU;
		if (buf_hPage->nxtLRU != NULL) buf_man->LRUidx->prvLRU = NULL;
	}

	while(i<BUFSIZE){
		table[i] = table[i+1];
		i++;
		if (i == BUFSIZE-1) {
			table[i] = -1;
			break;
		}
	}
	free(buf_hPage);
	close(table_id);
	return 0;
}

int shutdown_db(){
	for(int i=0; i<BUFSIZE; i++){
		if(table[i]>0) close_table(table[i]);
		else break;
	}
}

buffer_structure* buf_put_page(int table_id, pagenum_t pagenum){
	buffer_structure* new_buf = (buffer_structure*)malloc(sizeof(buffer_structure));
	if (pagenum == 0){
		header_page* hPage = (header_page*)calloc(1, PAGESIZE);
		pread(table_id, hPage, PAGESIZE, SEEK_SET);
		new_buf->hPage = hPage;
	}
	else {
		page_t* page = (page_t*)calloc(1,PAGESIZE);
		file_read_page(table_id, pagenum, page);
		new_buf->page = page;
	}
	new_buf->table_id = table_id;
    new_buf->page_num = pagenum;
    new_buf->is_dirty = false;
    new_buf->is_pinned = 0;
    new_buf->nxtLRU = NULL;
    new_buf->prvLRU = NULL;

	int num_buf = 0;
	buffer_structure* cnt = buf_man->LRUidx;
	if (cnt != NULL){
		while(cnt->nxtLRU != NULL){
			cnt = cnt->nxtLRU;
			num_buf++;
		}
	}

	if (num_buf == 0) { // empty buffer
		buf_man->LRUidx = new_buf;
		return new_buf;
	}

	// put in buffer
	cnt->nxtLRU = new_buf;
	new_buf->prvLRU = cnt;

	if (num_buf+1 >= buf_man->capacity){ // full buffer
		buffer_structure* tmp = drop_buf();
		buf_free_page(tmp);
	}

	return new_buf;
}

buffer_structure* drop_buf(){
	buffer_structure* tmp = buf_man->LRUidx;
	while(tmp->is_pinned!=0 && tmp!=NULL) tmp=tmp->nxtLRU;
	return tmp;
}

void buf_free_page(buffer_structure* buffer){
	if (buffer->is_dirty)
		file_write_page(buffer->table_id, buffer->page_num, buffer->page);
	else if (buffer->page->num_keys == 0)
		file_free_page(buffer);

	if (buffer->nxtLRU != NULL) buffer->nxtLRU->prvLRU = buffer->prvLRU;
	
	if (buffer->prvLRU != NULL) buffer->prvLRU->nxtLRU = buffer->nxtLRU;
	else {
		buf_man->LRUidx = buffer->nxtLRU;
		buf_man->LRUidx->prvLRU = NULL;
	}

	free(buffer->page);
	free(buffer);
}

buffer_structure* buf_read_page(int table_id, pagenum_t pagenum){
	buffer_structure* tmp = buf_man->LRUidx;
    while ( tmp != NULL ) {
        if ( tmp->table_id == table_id && tmp->page_num == pagenum ) break;
        tmp = tmp->nxtLRU;
    }
    if ( tmp == NULL ) 
        tmp = buf_put_page(table_id, pagenum);
    tmp->is_pinned++;
    return tmp;
}

buffer_structure* buf_write_page(buffer_structure* buffer, bool isDirty){
	if(!buffer->is_dirty) buffer->is_dirty = isDirty;
	(buffer->is_pinned)--;
}

buffer_structure* file_alloc_page(buffer_structure* buf_hPage){
	//printf("file alloc page...");

	page_t* new_freeP = (page_t *)calloc(1, PAGESIZE);
	pagenum_t tmpPN;
	buffer_structure* new_buf;

	header_page* hPage = buf_hPage->hPage;

	if (hPage->freePO != 0) { // free page exists
		//printf("free page exists -> file_read_page\n");
		tmpPN = hPage->freePO/4096;
		new_buf = buf_read_page(buf_hPage->table_id, tmpPN);
		hPage->freePO = new_buf->page->parentPO;
	}
	else { // make free page
		//printf("free page not exists -> new free page -> file_write_page\n");
		tmpPN = hPage->num_pages;
		file_write_page(buf_hPage->table_id, tmpPN, new_freeP);
		new_buf = buf_read_page(buf_hPage->table_id, tmpPN);
		(hPage->num_pages)++;
	}

	buf_write_page(buf_hPage, true);
	return new_buf;
}

void file_free_page(buffer_structure* buffer){
	buffer_structure* buf_hPage = buf_read_page(buffer->table_id, 0);
	page_t * tmpP = (page_t *)calloc(1, PAGESIZE);
	header_page* hPage = buf_hPage->hPage;

	tmpP->parentPO = hPage->freePO;
	hPage->freePO = (buffer->page_num)*4096;

	file_write_page(buffer->table_id, buffer->page_num, tmpP);
	buf_write_page(buf_hPage, true);

	free(tmpP);
}

void file_read_page(int now_tid, pagenum_t pagenum, page_t* dest){
	//printf("file_read_page ... pagenum:%"PRId64"\n",pagenum);
	int readsize = pread(now_tid, dest, PAGESIZE, pagenum*4096);
	if (readsize < PAGESIZE){
		printf("fail\n");
	}
}

void file_write_page(int now_tid, pagenum_t pagenum, const page_t* src){
	//printf("file_write_page ... pagenum:%"PRId64"\n",pagenum);
	int writesize = pwrite(now_tid, src, PAGESIZE, pagenum*4096);
	if (writesize < PAGESIZE){
		printf("fail\n");
	}
}

void usage( void ) {
	printf("***** 2017030137 Jane Yi b+tree *****\n\n");
	printf("Enter any of the following commands after the prompt > :\n"
	"\to <i> -- Open table <i> as table id.\n"
	"\to <i> -- Close table <i> as table id.\n"
    "\ti <i> <k> <v>  -- Insert <k> (int) as key and <v> (int *) as value in table <i>.\n"
    "\tf <i> <k>  -- Find the value under key <k> in table <i>.\n"
    "\td <i> <k>  -- Delete key <k> and its associated value in table <i>.\n"
	"\tj <q> -- Join table query. \n"
    "\tt <i> -- Print the B+ tree in table <i>.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n");
}

int64_t join(string query){
	return 0;
}

void enqueue(pagenum_t pagenum) {
    queue * enq = (queue *)malloc(sizeof(queue));
    queue * tmp;
    enq->nowPO = pagenum*4096;
    enq->nxt = NULL;
    // q is empty
    if ( qu == NULL ) qu = enq;
    
    // q has page
    else {
        tmp = qu;
        while( tmp->nxt != NULL ) 
            tmp = tmp->nxt;
        tmp->nxt = enq;
    }
}

pagenum_t dequeue() {
    queue * n = qu;
    pagenum_t rt = n->nowPO/4096;
    qu = qu->nxt;
    free(n);
    return rt;
}

void print_tree( int table_id ) {
	int i;
	for(i=0; i<BUFSIZE; i++)
		if(table[i]==table_id) break;
	if (i==BUFSIZE){
		printf("table_id not exists\n");
		return;
	}

    buffer_structure* buf_hPage = buf_read_page(table_id, 0);
    
    if ( buf_hPage->hPage->rootPO == 0 ) {
        printf("Empty tree.\n");
        return;
    }
    
    page_t* n = (page_t *)calloc(1, PAGESIZE);
    qu = (queue*)malloc(sizeof(queue));
    qu = NULL;
    enqueue(buf_hPage->hPage->rootPO/4096);

	pagenum_t nnum;
    while ( qu != NULL ) {
        nnum = dequeue();
        file_read_page(table_id, nnum, n);
        for ( i = 0; i < n->num_keys; ++i ) 
            printf("%"PRId64" ", n->is_leaf ? n->records[i].key : n->entries[i].key);
        if ( !n->is_leaf ) {
            enqueue(n->extraPO/4096);
            for ( i = 0; i < n->num_keys; ++i )
                enqueue(n->entries[i].entPO/4096);
        }
        printf("| ");
    }
    printf("\n");
    free(n);
    free(qu);
    buf_write_page(buf_hPage, false);
}

int64_t* find(int table_id, int64_t key){
	int i;
	for(i=0; i<BUFSIZE; i++)
		if(table[i]==table_id) break;
	if (i==BUFSIZE){
		printf("table_id not exists\n");
		return NULL;
	}

	pagenum_t fPnum = find_leaf(table_id, key);
	if (fPnum == 0) return NULL;

	buffer_structure* fbuf = buf_read_page(table_id, fPnum);

	int64_t* ans;

	for(i=0; i<fbuf->page->num_keys; i++)
		if (fbuf->page->records[i].key == key) break;

	if (i==fbuf->page->num_keys){
		buf_write_page(fbuf, false);
		ans =  NULL;
	}
	else ans = fbuf->page->records[i].value; 

	free(fbuf);
	return ans;
}

pagenum_t find_leaf(int table_id, int64_t key){
	buffer_structure* bufhPage = buf_read_page(table_id, 0);
	pagenum_t rPN = bufhPage->hPage->rootPO/4096;

	if (rPN == 0){
		buf_write_page(bufhPage, false);
		printf("Empty tree\n");
		return 0;
	}

	buffer_structure* fbuf = buf_read_page(table_id, rPN);
	if (fbuf->page->num_keys == 0){
		buf_write_page(fbuf, false);
		return 0;
	}

	while(!(fbuf->page->is_leaf)){
		int i=0;
		while(i<fbuf->page->num_keys){
			if (key >= fbuf->page->entries[i].key) i++;
			else break;
		}
		rPN = fbuf->page->entries[--i].entPO / 4096;
		buf_write_page(fbuf, false);
		fbuf = buf_read_page(table_id, fbuf->page->entries[i].entPO / 4096);
	}

	buf_write_page(bufhPage, false);
	buf_write_page(fbuf, false);
	return rPN;
}

int cut(int length){
	if (length % 2 == 0) return length/2;
	else return length/2 +1;
}

// insert success -> return  0 / fail -> return -1
int insert(int table_id, int64_t key, int64_t* value){
	int i;
	for(i=0; i<BUFSIZE; i++)
		if(table[i]==table_id) break;
	if (i==BUFSIZE){
		printf("table_id not exists\n");
		return -1; // insert fail
	}

	buffer_structure* bufhP = buf_read_page(table_id, 0);

	// no tree -> start new tree
	if (bufhP->hPage->rootPO == 0) {
		//printf(" ... start new tree\n");
		return start_new_tree(table_id, key, value);
	}

	buf_write_page(bufhP, false);

	// tree exists

	if (find(table_id, key) != NULL) {
		printf("key already exists\n");
		return -1; // insert fail
	}

	buffer_structure * leaf = buf_read_page(table_id, find_leaf(table_id, key));
	
		// leaf has room -> insert into leaf -> success
	if (leaf->page->num_keys < record_order - 1){
		//printf("insert into leaf\n");
		return insert_into_leaf(leaf, key, value);
	}
		// leaf split
	//printf("leaf split\n");
	return insert_into_leaf_after_splitting(leaf, key, value);
}

int start_new_tree(int table_id, int64_t key, int64_t* value){
	buffer_structure* bufhP = buf_read_page(table_id, 0);
	buffer_structure* rPage = make_leaf(table_id);

	bufhP->hPage->rootPO = rPage->page_num*4096;

	rPage->page->records[0].key = key;
	int ncol=bufhP->hPage->num_columns;
	for(int i=0; i<ncol-1; i++) (rPage->page->records[0].value)[i] = value[i];
	
	rPage->page->parentPO = 0;
	rPage->page->extraPO = 0;
	rPage->page->num_keys++;

	buf_write_page(bufhP, true);
	buf_write_page(rPage, true);

	return 0;
}

buffer_structure* make_leaf(int table_id){
	buffer_structure* leaf = make_node(table_id);
	leaf->page->is_leaf = true;
	return leaf;
}

buffer_structure * make_node(int table_id){
	buffer_structure* bufhP = buf_read_page(table_id, 0);
	buffer_structure* new_node = file_alloc_page(bufhP);
	new_node->page->is_leaf = false;
	new_node->page->num_keys = 0;
	new_node->page->parentPO = 0;
	new_node->page->extraPO = 0;
	return new_node;
}

int insert_into_leaf(buffer_structure* leaf, int64_t key, int64_t* value){
	//printf(" !! insert into leaf\n");

	int ins_pt = 0;
	while (ins_pt < leaf->page->num_keys && leaf->page->records[ins_pt].key < key) ins_pt++;

	buffer_structure* bufhP = buf_read_page(leaf->table_id, 0);
	int ncol = bufhP->hPage->num_columns;

	for (int i = leaf->page->num_keys; i > ins_pt; i--){
		leaf->page->records[i].key = leaf->page->records[i-1].key;
		for(int j=0; j<ncol-1; j++)
			(leaf->page->records[i].value)[j] = (leaf->page->records[i-1].value)[j];
	}
	leaf->page->records[ins_pt].key = key;
	for(int j=0; j<ncol-1; j++)
		(leaf->page->records[ins_pt].value)[j] = value[j];
	leaf->page->num_keys++;

	buf_write_page(leaf, true);
	return 0;
}

int insert_into_leaf_after_splitting(buffer_structure* leaf, int64_t key, int64_t* value){
	//printf(" !! insert into leaf after splitting\n");
	buffer_structure* new_leaf = make_leaf(leaf->table_id);

	buffer_structure* bufhP = buf_read_page(leaf->table_id, 0);
	int ncol = bufhP->hPage->num_columns;

   int64_t tmp_keys[record_order + 1];
   int64_t tmp_values[record_order + 1][15];

   int ins_idx = 0;
   while (ins_idx < record_order -1 && leaf->page->records[ins_idx].key < key) ins_idx++;
   //printf("ins_idx :%d\n",ins_idx);

   int i,j;
   for(i=0, j=0; i < leaf->page->num_keys; i++, j++){
      if (j==ins_idx) j++;
      tmp_keys[j] = leaf->page->records[i].key;
	  for(int k=0; k<ncol-1; k++) tmp_values[j][k] = (leaf->page->records[i]).value[k];
   }
   tmp_keys[ins_idx] = key;
   for(int k=0; k<ncol-1; k++) tmp_values[ins_idx][k] = value[k];
   leaf->page->num_keys = 0;

   int split = cut(record_order -1);
   //printf("split:%d\n",split);

   for(i=0; i<split; i++){
      leaf->page->records[i].key = tmp_keys[i];
	  for(int k=0; k<ncol-1; k++) (leaf->page->records[i].value)[k] = tmp_values[i][k];
      leaf->page->num_keys++;
   }
   
   for(i=split, j=0; i < record_order; i++, j++){
      new_leaf->page->records[j].key = tmp_keys[i];
	  for(int k=0; k<ncol-1; k++) (new_leaf->page->records[j].value)[k] = tmp_values[i][k];
      new_leaf->page->num_keys++;
   }

   new_leaf->page->parentPO = leaf->page->parentPO;
   //printf("new_leaf->parentPO = leaf->parentPO : %ld\n",leaf->parentPO);
   new_leaf->page->extraPO = leaf->page->extraPO;   
   leaf->page->extraPO = new_leaf->page_num*4096;
   
   int64_t new_key = new_leaf->page->records[0].key;
   return insert_into_parent(leaf, new_key, new_leaf);
}

int insert_into_parent(buffer_structure* left, int64_t key, buffer_structure* right){	
	//printf(" !! insert into parent\n");
	buffer_structure* parentP = buf_read_page(left->table_id, left->page->parentPO/4096);

	// new root
	if	(parentP == NULL){
		//printf("parent NULL -> new root\n");
		return insert_into_new_root(left, key, right);
	}

	int left_idx = -1;
	while(left_idx<parentP->page->num_keys && parentP->page->entries[left_idx].entPO!=left->page_num*4096)
		left_idx++;
	left_idx++;

	// new key fit
	if (parentP->page->num_keys < entry_order - 1){
		//printf("key fit in parent -> insert into node\n");
		buf_write_page(left, true);
		return insert_into_node(parentP, left_idx, key, right);
	}

	// node split
	buf_write_page(left, true);
	return insert_into_node_after_splitting(parentP, left_idx, key, right);
}

int insert_into_new_root(buffer_structure* left, int64_t key, buffer_structure* right){
	buffer_structure* bufhP = buf_read_page(left->table_id, 0);
	buffer_structure* root = make_node(left->table_id);
	bufhP->hPage->rootPO = root->page_num*4096;

	root->page->entries[0].key = key;
	root->page->num_keys++;
	root->page->parentPO = 0;
	root->page->extraPO = left->page_num*4096;
	root->page->entries[0].entPO = right->page_num*4096;

	left->page->parentPO = root->page_num*4096;
	buf_write_page(left, true);

	right->page->parentPO = root->page_num*4096;
	buf_write_page(right, true);

	buf_write_page(bufhP, true);
	buf_write_page(root, true);
	return 0;
}

int insert_into_node(buffer_structure* p, int ins_idx, int64_t key, buffer_structure* right){
	//printf(" !! insert into node ... ins_idx:%d, key:%ld, right:%ld\n",ins_idx,key,right);

	for(int i=p->page->num_keys; i>ins_idx; i--){
		p->page->entries[i].entPO = p->page->entries[i-1].entPO;
		p->page->entries[i].key = p->page->entries[i-1].key;
	}
	p->page->entries[ins_idx].entPO = right->page_num*4096;
	p->page->entries[ins_idx].key = key;
	p->page->num_keys++;

	buf_write_page(p, true);
	buf_write_page(right, true);
	return 0;
}

int insert_into_node_after_splitting(buffer_structure* old_node, int ins_idx, int64_t key, buffer_structure* right){
		//printf(" !! insert into node after splitting\n");

		// save to temp from old
		int64_t tmp_keys[entry_order];
		off_t tmp_offset[entry_order + 1];

		tmp_offset[0] = old_node->page->extraPO;
		for(int i=0,j=1; i<old_node->page->num_keys+1; i++,j++){
			if (j == ins_idx + 1) j++;
			tmp_offset[j] = old_node->page->entries[i].entPO;
		}
		for(int i=0,j=0; i<old_node->page->num_keys; i++,j++){
			if (j == ins_idx) j++;
			tmp_keys[j] = old_node->page->entries[i].key;
		}
		tmp_offset[ins_idx+1] = right->page_num*4096;
		tmp_keys[ins_idx] = key;

		buf_write_page(right, true);

		// create new & copy half to old, half to new
		buffer_structure* new_node = make_node(old_node->table_id);
		int split = cut(entry_order);
		old_node->page->num_keys = 0;

		int i, j;
		old_node->page->extraPO = tmp_offset[0];
		for(i=0; i<split-1; i++){
			old_node->page->entries[i].entPO = tmp_offset[i+1];
			old_node->page->entries[i].key = tmp_keys[i];
			old_node->page->num_keys++;
		}
		int k_prime = tmp_keys[split-1];
		new_node->page->extraPO = tmp_offset[split];
		for(i=split, j=0; i<entry_order; i++, j++){
			new_node->page->entries[j].entPO = tmp_offset[i+1];
			new_node->page->entries[j].key = tmp_keys[i];
			new_node->page->num_keys++;
		}
		
		// connect parent & child
		new_node->page->parentPO = old_node->page->parentPO;
		
		buffer_structure* childP;
		for (int i=0; i<new_node->page->num_keys; i++){
			childP = buf_read_page(new_node->table_id, new_node->page->entries[i].entPO/4096);
			childP->page->parentPO = new_node->page_num*4096;
			buf_write_page(childP, true);
		}
		return insert_into_parent(old_node, k_prime, new_node);
}

int remove(int table_id, int64_t key){
	int i;
	for(i=0; i<BUFSIZE; i++)
		if(table[i]==table_id) break;
	if (i==BUFSIZE){
		printf("table_id not exists\n");
		return -1; // delete fail
	}

	if (!find(table_id, key)) {
		printf("key not exists\n");
		return -1;
	}

	pagenum_t leafPN = find_leaf(table_id, key);
	buffer_structure* leafP = buf_read_page(table_id, leafPN);

	remove_entry(leafP, key);

	return 0;
}

void remove_entry(buffer_structure* nodeP, int64_t key){
	buffer_structure* bufhP = buf_read_page(nodeP->table_id, 0);
	int ncol = bufhP->hPage->num_columns;

	// remove key from node
	int i;
	if (nodeP->page->is_leaf){ // leaf
		i=0;
		while(nodeP->page->records[i].key != key) i++;
		for(++i; i<nodeP->page->num_keys; i++){
			nodeP->page->records[i-1].key = nodeP->page->records[i].key;
			for(int k=0; k<ncol-1; k++)
				(nodeP->page->records[i-1].value)[k] = (nodeP->page->records[i].value)[k];
		}
	}
	else {
		i=0;
		while(nodeP->page->entries[i].key != key) i++;
		for(++i; i<nodeP->page->num_keys; i++){
			nodeP->page->entries[i-1].key = nodeP->page->entries[i].key;
			nodeP->page->entries[i-1].entPO = nodeP->page->entries[i].entPO;
		}
	}
	nodeP->page->num_keys--;

	// case : deletion from root
	if (nodeP->page_num*4096 == bufhP->hPage->rootPO) {
		buf_write_page(bufhP, false);
		buf_write_page(nodeP, true);
		adjust_root(nodeP->table_id);
		return;
	}

	// case : deletion from node below root
	
	int min_keys = 1; // p->is_leaf ? cut(record_order-1) : cut(entry_order)-1;
	// delayed merge 니까

		// node stays at or above minimum
	if (nodeP->page->num_keys >= min_keys) {
		buf_write_page(bufhP, false);
		buf_write_page(nodeP, true);
		return;
	}

	buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);

		// node falls below minimum (coalescence / redistribution)
	int neighbor_idx = get_neighbor_index(nodeP);
	int k_prime_idx = (neighbor_idx == -1 ? 0 : neighbor_idx);
	int k_prime = parentP->page->entries[k_prime_idx].key;
	off_t neighborO = (neighbor_idx == -1 ? 
		parentP->page->entries[1].entPO : parentP->page->entries[neighbor_idx].entPO);

	buffer_structure* neighborP = buf_read_page(nodeP->table_id, neighborO/4096);

	int capacity = (nodeP->page->is_leaf ? record_order : entry_order-1);
	int tmp = neighborP->page->num_keys;

	buf_write_page(bufhP, false);
	buf_write_page(parentP, false);

	if(tmp < capacity) return coalesce_nodes(nodeP, neighborP, neighbor_idx, k_prime);
	else return redistribute_nodes(nodeP, neighborP, neighbor_idx, k_prime_idx, k_prime);

}

void remove_entry_from_node(buffer_structure* nodeP, int64_t key){
	if(nodeP->page->is_leaf){ // leaf
		buffer_structure* bufhP = buf_read_page(nodeP->table_id, 0);
		int ncol=bufhP->hPage->num_columns;

		int i=0;
		while (nodeP->page->records[i].key != key) i++;
		for(++i; i<nodeP->page->num_keys; i++) {
			nodeP->page->records[i-1].key = nodeP->page->records[i].key;
			for(int k=0; k<ncol-1; k++)
				(nodeP->page->records[i-1].value)[k] = (nodeP->page->records[i].value)[k];
		}
	}
	else { // internal
		int i=0;
		while (nodeP->page->entries[i].key != key) i++;
		for(++i; i<nodeP->page->num_keys; i++) {
			nodeP->page->entries[i-1].key = nodeP->page->entries[i].key;
			nodeP->page->entries[i-1].entPO = nodeP->page->entries[i].entPO;
		}
	}
	nodeP->page->num_keys--;
}

int get_neighbor_index(buffer_structure* nodeP){
	buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
	for(int i=0; i<=parentP->page->num_keys; i++){
		if (parentP->page->entries[i].entPO == nodeP->page_num*4096) {
			buf_write_page(parentP, false);
			return i-1;
		}
	}

	// error
	return -1;
}

void adjust_root(int table_id){
	buffer_structure* bufhP = buf_read_page(table_id, 0);
	buffer_structure* rPage = buf_read_page(table_id, bufhP->hPage->rootPO/4096);

	// nonempty root
	if(rPage->page->num_keys > 0) {
		buf_write_page(bufhP, true);
		buf_write_page(rPage, true);
		return;
	}
	
	// empty root
		// child exist -> first child as new root
	if(!rPage->page->is_leaf){
		bufhP->hPage->rootPO = rPage->page_num*4096;
		rPage = buf_read_page(table_id, rPage->page->extraPO/4096);
		buf_write_page(rPage, true);
		buf_write_page(bufhP, true);
	}
		// no child -> whole tree empty
	else {
		bufhP->hPage->rootPO = 0;
		buf_write_page(bufhP, true);
		buf_write_page(rPage, true);
	}
}

void coalesce_nodes(buffer_structure* nodeP, buffer_structure* neighborP, int neighbor_idx, int k_prime){

	// extreme left -> swap
	if(neighbor_idx == -1){
		buffer_structure* tmp = nodeP;
		nodeP = neighborP;
		neighborP = tmp;
	}

	int neighbor_insert_idx = neighborP->page->num_keys;

	// nonleaf node
	if (!nodeP->page->is_leaf){
		neighborP->page->entries[neighbor_insert_idx].key = k_prime;
		neighborP->page->num_keys++;

		int n_end = nodeP->page->num_keys;
		int i,j;

		for(i=neighbor_insert_idx+1, j=0; j<n_end; i++, j++){
			neighborP->page->entries[i].key = nodeP->page->entries[j].key;
			neighborP->page->entries[i].entPO = nodeP->page->entries[j].entPO;
			neighborP->page->num_keys++;
			nodeP->page->num_keys--;
		}
		neighborP->page->entries[i].entPO = nodeP->page->entries[j].entPO;

		for(i=0; i<neighborP->page->num_keys+1; i++){
			buffer_structure* childP = buf_read_page(nodeP->table_id, neighborP->page->entries[i].entPO/4096);
			childP->page->parentPO = neighborP->page_num*4096;
			buf_write_page(childP, true);
		}
	}

	// leaf
	else {
		buffer_structure* bufhP = buf_read_page(nodeP->table_id, 0);
		int ncol = bufhP->hPage->num_columns;

		for(int i=neighbor_insert_idx, j=0; j<nodeP->page->num_keys; i++, j++){
			neighborP->page->records[i].key = nodeP->page->records[j].key;
			for(int k=0; k<ncol-1; k++)
				(neighborP->page->records[i].value)[k] = (nodeP->page->records[j].value)[k];
			neighborP->page->num_keys++;
		}
		neighborP->page->extraPO = nodeP->page->extraPO;
	}

	buf_write_page(neighborP, true);
	buf_write_page(nodeP, true);

	buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
	return remove_entry(parentP, k_prime);
}

void redistribute_nodes(buffer_structure* nodeP, buffer_structure* neighborP, int neighbor_idx, int k_prime_idx, int k_prime){

	buffer_structure* bufhP = buf_read_page(nodeP->table_id, 0);
	int ncol = bufhP->hPage->num_columns;

	// neighbor to left
	if(neighbor_idx!=-1){
		if(!nodeP->page->is_leaf){ // internal
			nodeP->page->entries[nodeP->page->num_keys+1].entPO
			 = nodeP->page->entries[nodeP->page->num_keys].entPO;
			for(int i=nodeP->page->num_keys; i>0; i--){
				nodeP->page->entries[i].key = nodeP->page->entries[i-1].key;
				nodeP->page->entries[i].entPO = nodeP->page->entries[i-1].entPO;
			}
			nodeP->page->entries[0].entPO = neighborP->page->entries[neighborP->page->num_keys].entPO;

			buffer_structure* childP = buf_read_page(nodeP->table_id, nodeP->page->entries[0].entPO/4096);
			childP->page->parentPO = nodeP->page_num*4096;
			buf_write_page(childP, 4096);

			neighborP->page->entries[neighborP->page->num_keys].entPO = 0;
			nodeP->page->entries[0].key = k_prime;

			buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
			parentP->page->entries[k_prime_idx].key = neighborP->page->entries[neighborP->page->num_keys-1].key;
			buf_write_page(parentP, true);

			nodeP->page->num_keys++;
			neighborP->page->num_keys--;
		}
		else{ // leaf
			for(int i=nodeP->page->num_keys; i>0; i--){
				nodeP->page->records[i].key = nodeP->page->records[i-1].key;
				for(int k=0; k<ncol-1; k++)
					(nodeP->page->records[i].value)[k] = (nodeP->page->records[i-1].value)[k];
			}
			for(int k=0; k<ncol-1; k++)
				(nodeP->page->records[0].value)[k] = (neighborP->page->records[neighborP->page->num_keys -1].value)[k];
			for(int k=0; k<ncol-1; k++)
				(neighborP->page->records[neighborP->page->num_keys -1].value)[k] = NULL;
			nodeP->page->records[0].key = neighborP->page->records[neighborP->page->num_keys -1].key;

			buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
			parentP->page->entries[k_prime_idx].key = nodeP->page->records[0].key;

			nodeP->page->num_keys++;
			neighborP->page->num_keys--;
		}
	}
	// leftmost child
	else{
		if(nodeP->page->is_leaf){ // leaf
			nodeP->page->records[nodeP->page->num_keys].key = neighborP->page->records[0].key;
			for(int k=0; k<ncol-1; k++)
				(nodeP->page->records[nodeP->page->num_keys].value)[k] = (neighborP->page->records[0].value)[k];

			buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
			parentP->page->entries[k_prime_idx].key = neighborP->page->records[1].key;
		
			for(int i=0; i<neighborP->page->num_keys-1; i++){
				neighborP->page->records[i].key = neighborP->page->records[i+1].key;
				for(int k=0; k<ncol-1; k++)
					(neighborP->page->records[i].value)[k] = (neighborP->page->records[i+1].value)[k];
			}

			nodeP->page->num_keys++;
			neighborP->page->num_keys--;
		}
		else{ // internal
			nodeP->page->entries[nodeP->page->num_keys].key = k_prime;
			nodeP->page->entries[nodeP->page->num_keys+1].entPO = neighborP->page->entries[0].entPO;
			
			buffer_structure* childP = buf_read_page(nodeP->table_id, nodeP->page->entries[nodeP->page->num_keys +1].entPO/4096);
			childP->page->parentPO = nodeP->page_num*4096;

			buffer_structure* parentP = buf_read_page(nodeP->table_id, nodeP->page->parentPO/4096);
			parentP->page->entries[k_prime_idx].key = neighborP->page->entries[0].key;

			int i;
			for(i=0; i<neighborP->page->num_keys -1; i++){
				neighborP->page->entries[i].key = neighborP->page->entries[i+1].key;
				neighborP->page->entries[i].entPO = neighborP->page->entries[i+1].entPO;
			}
			neighborP->page->entries[i].entPO = neighborP->page->entries[i+1].entPO;
		}
	}
	free(nodeP);
	free(neighborP);
}