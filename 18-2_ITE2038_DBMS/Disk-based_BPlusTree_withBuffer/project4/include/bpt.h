#ifndef __BPT_H__
#define __BPT_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdbool.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
using namespace std;

#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

#define PAGESIZE 4096 // bytes
#define record_order 32
#define entry_order 249
#define BUFSIZE 10

typedef struct record { // leaf page : records
	int64_t key;
    int64_t value[15];
} record;

typedef struct entry { // internal page : entries
	int64_t key;
	off_t entPO;
} entry;

// use queue to print tree
typedef struct queue {
	off_t nowPO;
	struct queue * nxt;
} queue;

queue * qu;

/**** page ****/

typedef uint64_t pagenum_t;

typedef struct page_t { // in-memory page structure
	off_t parentPO;
	int is_leaf;
	int num_keys;
	char reserved[104];
	off_t extraPO;
	
	union {
		// internal, leaf, free page : share
		entry entries[248];
		record records[31];
	};
} page_t;

typedef struct header_page {
	off_t freePO;
	off_t rootPO;
	int64_t num_pages;
	int64_t num_columns;
	char reserved[4072];
} header_page;

/**** buffer ****/

typedef struct buffer_structure{
	union{
		header_page* hPage;
		page_t* page;
	};
	int table_id;
	pagenum_t page_num;
	bool is_dirty;
	int is_pinned;
	struct buffer_structure* nxtLRU;
	struct buffer_structure* prvLRU;
} buffer_structure;

typedef struct buffer_manager{
	int capacity;
	buffer_structure* LRUidx;
} buffer_manager;

int table[10];

int init_db(int num_buf);
// Allocate the buffer pool (array) with the given number of entries.
// Initialize other fields (state info, LRU info..) with your own design.
// If success, return 0. Otherwise, return non-zero value.

int open_table(char *pathname, int num_column);
// Open existing data file or create one if not existed.
// If success, return the unique table id, which represents the own table in this database.
// Return negative value if error occurs

int close_table(int table_id);
// Write all pages of this table from buffer to disk and discard the table id.
// If success, return 0. Otherwise, return non-zero value.

int shutdown_db();
// Flush all data from buffer and destroy allocated buffer.
// If success, return 0. Otherwise, return non-zero value.


buffer_structure* buf_put_page(int table_id, pagenum_t pagenum);
// put new page into buffer

buffer_structure* drop_buf();
// return page in buffer to drop (when buffer is full) (select by LRU)

void buf_free_page(buffer_structure* buf);
// free page in buffer and write page in disk

buffer_structure* buf_read_page(int table_id, pagenum_t pagenum);
// read page in buffer

buffer_structure* buf_write_page(buffer_structure* buffer, bool isDirty);
// write page in buffer (not new page)

buffer_structure* file_alloc_page(buffer_structure* buf_hPage);
void file_free_page(buffer_structure* buffer);
void file_read_page(int now_tid, pagenum_t pagenum, page_t* dest);
void file_write_page(int now_tid, pagenum_t pagenum, const page_t* src);

/**** join ****/
int64_t join(string query);


/**** Output and utility ****/ 

void usage( void );
void enqueue(pagenum_t pagenum);
pagenum_t dequeue();
int path_to_root( off_t rootPO, off_t childPO);
void print_tree(int table_id);

int cut( int length );

pagenum_t find_leaf(int table_id, int64_t key);
int64_t* find(int table_id, int64_t key);
// Find the record containing input ‘key’.
// If found matching ‘key’, return matched ‘value’ string.Otherwise, return NULL.


/**** Insert ****/

int insert(int table_id, int64_t key, int64_t* values);
// Insert input ‘key / value’(record) to data file at the right place.
// If success, return 0. Otherwise, return non - zero value.

int start_new_tree(int table_id, int64_t key, int64_t* value);
buffer_structure* make_node(int table_id);
buffer_structure* make_leaf(int table_id);
int insert_into_leaf(buffer_structure* leaf, int64_t key, int64_t* value);
int insert_into_leaf_after_splitting(buffer_structure* leaf, int64_t key, int64_t* value);
int insert_into_parent(buffer_structure* left, int64_t key, buffer_structure* right);
int insert_into_new_root(buffer_structure* left, int64_t key, buffer_structure* right);
int insert_into_node(buffer_structure* parent, int left_idx, int64_t key, buffer_structure* right);
int insert_into_node_after_splitting(buffer_structure* parent, int left_idx, int64_t key, buffer_structure* right);


/**** Delete ****/
int remove(int table_id, int64_t key);
// Find the matching record and delete it if found.
// If success, return 0. Otherwise, return non - zero value.

void remove_entry(buffer_structure* nodeP, int64_t key);
int get_neighbor_index(buffer_structure* nodeP);
void adjust_root(int table_id);
void coalesce_nodes(buffer_structure* nodeP, buffer_structure* neighborP, int neighbor_idx, int k_prime);
void redistribute_nodes(buffer_structure* nodeP, buffer_structure* neighborP, int neighbor_idx, int k_prime_idx, int k_prime);

#endif /* __BPT_H__ */