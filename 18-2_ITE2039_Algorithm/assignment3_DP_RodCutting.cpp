#include <stdio.h>
int n;			// length of Rod
int val[110];	// val[i] : value when length is i

int rev[110];	// rev[i] : max revenue from 1 ~ i (dp) 
int save[110];	// save length when max

int max(int a, int b) { return(a > b ? a : b); }

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &val[i]);

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			if (rev[i] < val[j] + rev[i - j]) {
				rev[i] = max(rev[i], val[j] + rev[i - j]);
				save[i] = j;
			}
		}
	}

	printf("%d\n", rev[n]);
	while (n > 0) {
		printf("%d ", save[n]);
		n -= save[n];
	}

	return 0;
}