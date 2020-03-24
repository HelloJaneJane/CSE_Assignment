#include <stdio.h>

int n, m;

int arr[30010];

void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++) scanf("%d", &arr[i]);

	for (int i = 1; i <= m; i++) {
		int key = i;
		for (int j = i + 1; j <= n; j++) {
			if (arr[j] < arr[key]) key = j;
		}
		swap(arr[i], arr[key]);
	}

	for (int i = 1; i <= n; i++) printf("%d\n", arr[i]);

	return 0;
}