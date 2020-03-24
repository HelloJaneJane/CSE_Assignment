#include "bpt.h"
#include "bpt.cpp"

int main( int argc, char ** argv ) {

    usage();

	int tid;
    int64_t input;
	int num_col;
    char instruction;
	char* pathname = (char*)malloc(100);
	int64_t* values;
	int64_t* find_val;
	string query;
	int64_t ret;

	init_db(BUFSIZE);
    
    printf("> ");
    while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'o':{
			scanf("%s %d", pathname, &num_col);
			tid = open_table(pathname, num_col);
			if (tid <= 0) printf("open table fail\n");
			else printf("open table ... now tid : %d\n", tid);
			break;
		}
		case 'c':{
			scanf("%d", &tid);
			if (close_table(tid) == 0) printf("close table success\n");
			else printf("close table fail\n");
			break;
		}
		case 'd':{
			scanf("%d%"PRId64"", &tid, &input);
			if (remove(tid, input) == 0) printf("delete success\n");
			else printf("delete fail\n");
			break;
		}
        case 'i':{
			scanf("%d%"PRId64"", &tid, &input);
			buffer_structure* tmphP = buf_read_page(tid, 1);
			int ncol = tmphP->hPage->num_columns;
			free(tmphP);
		    values = (int64_t*)malloc(sizeof(int64_t)*(ncol-1));
			for(int i=0; i<ncol-1; i++) scanf("%"PRId64"", &values[i]);
			
			if (insert(tid, input, values) == 0) printf("insert success\n");
			else printf("insert fail\n");
			break;
		}
		case 'f':{
			scanf("%d%"PRId64"", &tid, &input);
			find_val = find(tid, input);
			if (find_val) printf("%ld\n", &find_val);
			else printf("not exist\n");
			break;
		}
		case 'j':{
			cin >> query;
			ret = join(query);
			printf("%"PRId64"", ret);
			break;
		}
        case 't':{
		 	scanf("%d", &tid);
            print_tree(tid);
            break;
		}
		case 'q':{
			while (getchar() != (int)'\n');
			shutdown_db();
			return EXIT_SUCCESS;
			break;
		}
		}
		while (getchar() != (int)'\n');
        printf("> ");
	}
	printf("\n");
	shutdown_db();
	return EXIT_SUCCESS;
}
