#include <stdio.h>

#define parent i/2
#define Lchild 2*i
#define Rchild 2*i+1

int n, k;
int arr[100010];

int size;

int max(int a, int b) { return (a > b) ? a : b; }

void swap(int& a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
	//printf("swap(%d,%d)\n", a, b);
}

// max heapify
void heapify(int i) {
	if (Lchild > size) return; // (자식 둘 다 x) leaf면 끝
	if (Rchild > size) arr[Rchild] = 0; // 자식 하나면 오른쪽 초기화

	//printf("heapify(%d)\n", i);
	
	int tmp = max(arr[Lchild], arr[Rchild]);

	if (tmp > arr[i]) {
		if (tmp == arr[Lchild]) {
			swap(arr[Lchild], arr[i]);
			heapify(Lchild);
		}
		else if (tmp == arr[Rchild]) {
			swap(arr[Rchild], arr[i]);
			heapify(Rchild);
		}
	}
}

// build max heap
void build() {
	size = n;
	for (int i = size / 2; i >= 1; i--) {
		heapify(i);
	}
}

void heapsort(int k) {
	build();
	for (int i = size; i >= 2; i--) {
		if (k == 0) {
			printf("\n");
			break;
		}

		swap(arr[1], arr[size]);
		printf("%d ", arr[size--]);
		k--;
		heapify(1);
	}
}


int main() {
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++) scanf("%d", &arr[i]);
	heapsort(k);
	for (int i = 1; i <= n - k; i++) printf("%d ", arr[i]);
	printf("\n");
}