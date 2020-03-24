#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct node* Node;
typedef struct node* Tree;
typedef struct node {
	int value;
	Node parent;
	Node leftChild;
	Node rightChild;
}node;

Tree createTree() {
	Tree tmp = (Tree)malloc(sizeof(struct node));
	tmp->value = 0;
	tmp->leftChild = NULL;
	tmp->rightChild = NULL;
	tmp->parent = NULL;
	return tmp;
}

int isEmpty(Tree t) {
	if (t->rightChild == NULL) return 1;
	return 0;
}

Node search(Tree t, int key) {
	if (t == NULL) return t;
	if (key > t->value) return search(t->rightChild, key);
	else if (key < t->value) return search(t->leftChild, key);
	else return t;
}

// dif가 0이면 parent의 leftchild, 1이면 rightchild
void deleteNode(Tree t, int key) {
	Node del = search(t, key);
	int dif = 0;
	if (del == NULL) {
		printf("Not Found\n");
		return;
	}
	if (del->parent->leftChild == del) dif = 0;
	else dif = 1;

	Node tmp;
	int value = 0;
	if (del->leftChild == NULL && del->rightChild == NULL) {
		// 자식이 없는 경우
		del->value = 0;
	}
	else if (del->leftChild != NULL && del->rightChild == NULL) {
		// 왼쪽 자식만 있는 경우 왼쪽 자식을 부모에 연결
		del->leftChild->parent = del->parent;
		if (dif) del->parent->rightChild = del->leftChild;
		else del->parent->leftChild = del->leftChild;
		del->value = 0;
	}
	else if (del->leftChild == NULL && del->rightChild != NULL) {
		// 오른쪽 자식만 있는 경우 오른쪽 자식을 부모에 연결
		del->rightChild->parent = del->parent;
		if (dif) del->parent->rightChild = del->rightChild;
		else del->parent->leftChild = del->rightChild;
		del->value = 0;
	}
	else {
		// 양쪽 자식이 다 있는 경우 왼쪽 서브 트리 중 가장 큰 값 올림
		tmp = del->leftChild;
		while (tmp) {
			value = tmp->value;
			tmp = tmp->rightChild;
		}
		deleteNode(t, value);
		del->value = value;
	}
}

void insertNode(Tree t, int key) {
	Node parent = { 0 };
	Node tmp;
	
	int dif = 0;
	if (isEmpty(t)) {
		tmp = createTree();
		tmp->value = key;
		tmp->parent = t;
		t->rightChild = tmp;
		return;
	}
	else {
		while (t) {
			parent = t;
			if (t->value > key) {
				dif = 0;
				t = t->leftChild;
			}
			else {
				dif = 1;
				t = t->rightChild;
			}
		}
		t = createTree();
		t->value = key;
		t->parent = parent;
		if (dif == 1) {
			parent->rightChild = t;
		}
		else { // dif == 0
			parent->leftChild = t;
		}
		return;
	}
}

void showAll(Tree t) {
	if (t) {
		showAll(t->leftChild);
		if (t->value) printf("%d ", t->value);
		showAll(t->rightChild);
	}
}

int recursiveHeight(Tree t, int h) {
	if (!t) return h - 1;
	return MAX(recursiveHeight(t->leftChild, h + 1), recursiveHeight(t->rightChild, h + 1));
}
// return -1 if error, else return height of key node.
int getHeight(Tree t, int key) {
	int h = 0;
	Node cur = t;
	while (1) {
		if (!cur) return -1;
		if (cur->value == key) break;
		else if (cur->value > key) cur = cur->leftChild;
		else cur = cur->rightChild;
	}
	return recursiveHeight(cur, 0);
}


int main() {
	Tree T = createTree();

	while (1) {
		char c;
		scanf("%c", &c);
		if (c == 'e') break;
		else if (c == 's') {
			showAll(T);
			printf("\n");
		}
		else {
			int n;
			scanf("%d", &n);

			if (c == 'i') insertNode(T, n);
			else if (c == 'd') deleteNode(T, n);
			else if (c == 'h') {
				int a = getHeight(T, n);
				if (a == -1) printf("Not Found\n");
				else printf("The height of the node (%d) is %d\n", n, a);
			}
		}
	}
	return 0;
}