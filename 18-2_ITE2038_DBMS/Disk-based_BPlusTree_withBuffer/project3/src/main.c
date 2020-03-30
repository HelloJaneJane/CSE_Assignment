#include "bpt.h"
#include "bpt.c"

int main( int argc, char ** argv ) {

    usage();

    int64_t input;
    char instruction;
    char *buffer;
    buffer = malloc(120);
    open_db("bpt.db");
    
    printf("> ");
    while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'd':
			scanf("%"PRId64, &input);
			//delete(input);
			break;

        case 'i':
			scanf("%"PRId64" %s", &input, buffer);
			int buf = insert(input, buffer);
			if (buf == 0) printf(" !! insert success\n");
			else printf(" !! insert fail\n");

			print_tree();
			break;

		case 'f':
			scanf("%"PRId64, &input);
			buffer = find(input);
			if (buffer) printf("%s\n", buffer);
			else printf("Not Exists\n");
			fflush(stdout);
			break;

         case 't':
            print_tree();
            break;
			
		case 'q':
			while (getchar() != (int)'\n');
			return EXIT_SUCCESS;
			break;
		}
		while (getchar() != (int)'\n');
        printf("> ");
	}
	printf("\n");

	return EXIT_SUCCESS;
}