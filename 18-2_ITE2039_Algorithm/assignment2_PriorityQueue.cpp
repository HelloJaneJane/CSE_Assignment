#include <stdio.h>
#include <vector>
using namespace std;

#define parent i/2
#define Lchild 2*i
#define Rchild 2*i+1

int pq[1000010];
vector<int> ans;

void swap(int& a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int heapsize;

// max heapify
void heapify(int i) {
	// child 없을 때
	if (Lchild > heapsize) return;

	int tmp = 0; // 1이면 L이랑, 2이면 R이랑 swap (0이면 swap X)

	// Lchild만 있을 때
	if (Rchild > heapsize) {
		if (pq[Lchild] > pq[i]) tmp = 1;
	}
	// Lchild, Rchild 둘 다 있을 때
	else {
		if (pq[Lchild] > pq[Rchild] && pq[Lchild] > pq[i]) tmp = 1;
		else if (pq[Lchild] <= pq[Rchild] && pq[Rchild] > pq[i]) tmp = 2;
	}

	if (tmp == 1) {
		swap(pq[Lchild], pq[i]);
		heapify(Lchild);
	}
	else if (tmp == 2) {
		swap(pq[Rchild], pq[i]);
		heapify(Rchild);
	}
}

int main() {

	while (true) {
		int n;
		scanf("%d", &n);

		if (n == 1) {
			int a;
			scanf("%d", &a);
			pq[++heapsize] = a;
			// build
			for (int i = heapsize / 2; i >= 1; i /= 2) heapify(i);
		}
		else if (n == 2) {
			// extract-max
			swap(pq[1], pq[heapsize]);
			ans.push_back(pq[heapsize--]);
			heapify(1);
		}
		else if (n == 3) {
			int a, b;
			scanf("%d%d", &a, &b);
			pq[a] = b;

			if (pq[a] > b) {
				pq[a] = b;
				heapify(a);
			}
			else {
				pq[a] = b;
				for (int i = a; i >= 1; i /= 2) heapify(i);
			}

		}
		else {
			for (int i = 0; i < ans.size(); i++) printf("%d ", ans[i]);
			printf("\n");
			for (int i = 1; i <= heapsize; i++) printf("%d ", pq[i]);
			break;
		}
	}
	return 0;
}