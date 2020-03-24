#include <stdio.h>
#include <stdlib.h>

typedef struct node * pNode;
typedef pNode List;
typedef pNode Node;

struct node {
	int value;
	Node next;
};

List makeEmptyList(List L);
void insert(int v, List L, Node N);
Node findPrev(int v, List L);
void delete_one(int v, List L);
Node find(int v, List L);

List makeEmptyList(List L) {
	L->value = 0;
	L->next = NULL;
	return L;
}
void insert(int v, List L, Node N) {
	Node tmp;
	tmp = (Node)malloc(sizeof(struct node));
	tmp->value = v;
	tmp->next = N->next;
	N->next = tmp;
}
Node findPrev(int v, List L) {
	Node N;
	N = L;
	while (N->next != NULL && N->next->value != v) {
		N = N->next;
	}
	return N;
}
void delete_one(int v, List L) {
	Node N, tmp;
	N = findPrev(v, L);

	if (N->next != NULL) { // 맨 뒤가 아니면
		tmp = N->next;
		N->next = tmp->next;
		free(tmp);	
	}
}
Node find(int v, List L) {
	Node N;
	N = L->next;
	while (N != NULL && N->value != v) {
		N = N->next;
	}
	return N; // N이 NULL이면 v가 L에 없는 거 (에러)
}


int main() {

	List Li;
	Li = (List)malloc(sizeof(struct node));
	makeEmptyList(Li);

	while (1) {
		char cmd;
		scanf("%c", &cmd);

		if (cmd == 'e') break;

		if (cmd == 'i') {
			int a, b;
			scanf("%d%d", &a, &b);
			Node N = find(b, Li);
			if (b == 0) N = Li;
			if (N == NULL) {
				printf("Insert error. The %d isn't in the list.\n", b);
			}
			else {
				insert(a, Li, N);
			}
		}else if (cmd == 's') {
			Node N = Li;
			while (N->next != NULL) {
				N = N->next;
				printf("%d ", N->value);
			}
			printf("\n");
		}
		else if (cmd == 'd') {
			int a;
			scanf("%d", &a);
			Node N = find(a, Li);
			if (N == NULL) {
				printf("Delete error. The %d isn't in the list.\n", a);
			}
			else {
				delete_one(a, Li);
			}
		}
		else if (cmd == 'f') {
			int a;
			scanf("%d", &a);
			Node N = find(a, Li);
			if (N == NULL) {
				printf("Find error. The %d isn't in the list.\n", a);
			}
			else {
				N = findPrev(a, Li);
				int b = N->value;
				if (b == 0) printf("The %d is next of The header.\n", a);
				else printf("The %d is next of The %d.\n", a, b);
			}
		}
	}
		return 0;
}
