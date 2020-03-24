#include <stdio.h>

int N;
int arr[100010];
int sort[100010];

void merge(int l, int m, int r) {
	int i1 = l;		// l ~ m
	int i2 = m + 1; // m+1 ~ r

	int k = l;

	while (i1 <= m && i2 <= r) { // 둘 중 작은거를 sort[k]에 넣는다
		if (arr[i1] <= arr[i2]) {
			sort[k] = arr[i1++];
		}
		else {
			sort[k] = arr[i2++];
		}
		k++;
	}

	if (i1 > m) { // i2쪽이 남았을 때
		for (int i = i2; i <= r; i++) {
			sort[k++] = arr[i];
		}
	}
	else { // i1쪽이 남았을 때
		for (int i = i1; i <= m; i++) {
			sort[k++] = arr[i];
		}
	}

	for (int i = l; i <= r; i++) arr[i] = sort[i];
}

void mergesort(int l, int r) {
	if (l < r) {
		int m = (l + r) / 2;
		mergesort(l, m);
		mergesort(m + 1, r);
		merge(l, m, r);
	}
}


int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) scanf("%d", &arr[i]);

	mergesort(0, N - 1);

	for (int i = N - 1; i >= 0; i--) printf("%d\n", arr[i]);

	return 0;
}