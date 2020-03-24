#ifndef __BPT_H__
#define __BPT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

#define PAGESIZE 4096 // bytes
#define record_order 32
#define entry_order 249

typedef struct record { // leaf page : records
	int64_t key;
    char value[120];
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
	char reserved[4072];
} header_page;

typedef struct buffer_structure{
	union{
		header_page* hPage;
		page_t* page;
	};
	int table_id;
	pagenum_t page_num;
	bool isDirty;
	int pin_cnt;
	int nxtLRU;
	int prvLRU;
} buffer_structure;

typedef struct buffer_manager{
	int capacity;
	int LRUidx;
	buffer_structure * buffer_pool;
} buffer_manager;


int init_db(int num_buf);
// Allocate the buffer pool (array) with the given number of entries.
// Initialize other fields (state info, LRU info..) with your own design.
// If success, return 0. Otherwise, return non-zero value.

int open_table(char *pathname);
// Open existing data file or create one if not existed.
// If success, return the unique table id, which represents the own table in this database.
// Return negative value if error occurs

int close_table(int table_id);
// Write all pages of this table from buffer to disk and discard the table id.
// If success, return 0. Otherwise, return non-zero value.

int shutdown_db();
// Flush all data from buffer and destroy allocated buffer.
// If success, return 0. Otherwise, return non-zero value.


// Allocate an on-disk page from the free page list
pagenum_t file_alloc_page();

// Free an on-disk page to the free page list
void file_free_page(pagenum_t pagenum);

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t* dest);

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t* src);



/**** Output and utility ****/ 

void usage( void );
void enqueue( off_t newPO );
off_t dequeue( void );
int path_to_root( off_t rootPO, off_t childPO);
void print_tree();

int cut( int length );

pagenum_t find_leaf(page_t * fP, int64_t key);
char * find(int table_id, int64_t key);
// Find the record containing input ‘key’.
// If found matching ‘key’, return matched ‘value’ string.Otherwise, return NULL.


/**** Insert ****/

int insert(int table_id, int64_t key, char * value);
// Insert input ‘key / value’(record) to data file at the right place.
// If success, return 0. Otherwise, return non - zero value.

int start_new_tree(int table_id, int64_t key, char * value);
page_t * make_node (page_t * node);
page_t * make_leaf(page_t * leaf);
int insert_into_leaf(int table_id, pagenum_t leafnum, int64_t key, char * value);
int insert_into_leaf_after_splitting(int table_id, pagenum_t leafnum, int64_t key, char * value);
int insert_into_parent(int table_id, pagenum_t left, int64_t key, pagenum_t right);
int insert_into_new_root(int table_id, pagenum_t left, int64_t key, pagenum_t right);
int insert_into_node(int table_id, pagenum_t parent, int left_idx, int64_t key, pagenum_t right);
int insert_into_node_after_splitting(int table_id, pagenum_t parent, int left_idx, int64_t key, pagenum_t right);


/**** Delete ****/
int delete (int table_id, int64_t key);
// Find the matching record and delete it if found.
// If success, return 0. Otherwise, return non - zero value.

void delete_entry(int table_id, pagenum_t nodePN, int64_t key);
int get_neighbor_index(int table_id, pagenum_t nodePN);
void adjust_root(int table_id);
page_t * coalesce_nodes(int table_id, pagenum_t nodePN, pagenum_t neighborPN, int neighbor_idx, int k_prime);
page_t * redistribute_nodes(int table_id, pagenum_t nodePN, pagenum_t neighborPN, int neighbor_idx, int k_prime_idx, int k_prime);

#endif /* __BPT_H__ */