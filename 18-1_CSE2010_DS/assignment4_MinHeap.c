#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define MAX_ELEMENTS 100000
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
	int key;
} element;
element heap[MAX_ELEMENTS];

int N = 0;
element heap0 = { 0 };

void insert_min_heap(element item) {
	int i;
	if (HEAP_FULL(N)) {
		printf("The heap is full.\n");
		return;
	}
	i = ++N;

	while ((i != 1) && (abs(item.key) <= heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	
	heap[i] = item;
}

element delete_min_heap() {
	element item, temp;
	if (HEAP_EMPTY(N)) {
		return heap0;
	}
	item = heap[1];
	temp = heap[N--];
	int parent = 1;
	int child = 2;
	while (child <= N) {
		if ((child < N) && (abs(heap[child].key) > abs(heap[child + 1].key)))
			child++;
		if (abs(temp.key) < abs(heap[child].key)) break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;
	return item;
}

int main() {
	int q;
	scanf("%d", &q);

	while (q--) {
		int a;
		scanf("%d", &a);

		if (a != 0) {
			element item = { a };
			insert_min_heap(item);
		}
		else {
			element ans = delete_min_heap();
			printf("%d\n", ans.key);
		}
	}
}