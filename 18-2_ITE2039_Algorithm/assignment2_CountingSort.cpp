#include <stdio.h>

int n, m, k;

int A[100010];
int B[100010];

int key[100010];
int count[100010];

int main() {
	scanf("%d%d%d", &n, &m, &k);

	for (int i = 1; i <= k; i++) scanf("%d%d", &A[i], &B[i]);

	for (int i = 1; i <= n; i++) {
		scanf("%d", &key[i]);
		count[key[i]]++;
	}
	for (int i = 2; i <= m; i++) count[i] += count[i - 1];
	
	for (int i = 1; i <= k; i++) printf("%d\n", count[B[i]] - count[A[i] - 1]);

	return 0;
}