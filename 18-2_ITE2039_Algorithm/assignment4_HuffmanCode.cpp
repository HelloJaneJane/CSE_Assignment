#include <stdio.h>
#include <stdlib.h>

int n;

int min(int a, int b) { return (a < b) ? a : b; }

typedef struct node {
	int freq;
	node* Lchild;
	node* Rchild;
} node;

node* make_node(int a) {
	node * n = (node*)malloc(sizeof(node));
	n->freq = a;
	n->Lchild = NULL;
	n->Rchild = NULL;
	return n;
}

// fixed-length
int fixed_len(int total, int n) {
	if (n == 1) return total;

	n = n - 1;
	int cnt = 0;
	while (n > 0) {
		n /= 2;
		cnt++;
	}
	return cnt * total;
}

// min-heap
int size;
node * min_heap[30010];


void swap(node* &a, node* &b) {
	node* tmp = a;
	a = b;
	b = tmp;
}

void heapify(int i) {
	int Lchild = 2 * i;
	int Rchild = 2 * i + 1;

	if (Lchild > size) {
		return; // (자식 둘 다 x) leaf면 끝
	}

	if (Rchild > size) { // Lchild만 있을 때
		if (min_heap[Lchild]->freq < min_heap[i]->freq) {
			swap(min_heap[Lchild], min_heap[i]);
			heapify(Lchild);
		}
	}
	else { // Lchild, Rchild 다 있을 때
		int l = min_heap[Lchild]->freq;
		int r = min_heap[Rchild]->freq;
		int f = min_heap[i]->freq;

		if (l < f && l <= r) {
			swap(min_heap[Lchild], min_heap[i]);
			heapify(Lchild);
		}
		else if (r < f && r <= l) {
			swap(min_heap[Rchild], min_heap[i]);
			heapify(Rchild);
		}
	}
}

void build() {
	size = n;
	for (int i = size / 2; i >= 1; i--)
		heapify(i);
}

void insert(node *n) {
	min_heap[++size] = n;
	if (size == 1) return;

	int tmp = size;
	while (tmp > 1) {
		heapify(tmp /= 2);
	}
}

node* extract_min() {
	if (size == 1) {
		size = 0;
		return min_heap[1];
	}

	swap(min_heap[1], min_heap[size--]);
	heapify(1);
	return min_heap[size + 1];
}

// huffman code
bool isleaf(node *n) {
	return (n->Lchild == NULL && n->Rchild == NULL ? true : false);
}

int ans;

void traversal(node *n, int cnt) {
	if (isleaf(n)) {
		ans += n->freq * cnt;
		return;
	}
	traversal(n->Lchild, cnt + 1);
	traversal(n->Rchild, cnt + 1);
}


int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		char c[5];	// character input (???????안해도될거같은디)
		scanf("%s", c);

		int a;
		scanf("%d", &a);	// number of character

		node* newN = make_node(a);

		min_heap[i] = newN;
	}
	int total;
	scanf("%d", &total);

	// fixed-length code
	printf("%d\n", fixed_len(total, n));

	// huffman code
	build();

	node * root;
	while (true) {
		node * L = extract_min();

		if (size == 0) {
			root = L;
			break;
		}

		node * R = extract_min();
		node * Z = make_node(L->freq + R->freq);
		Z->Lchild = L;
		Z->Rchild = R;
		insert(Z);

	}

	traversal(root, 0);
	printf("%d\n", ans);

	return 0;
}