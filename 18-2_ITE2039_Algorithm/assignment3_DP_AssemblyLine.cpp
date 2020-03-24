#include <stdio.h>

int n, e1, e2, x1, x2;
int a[3][110];
int t[3][110];

int dp[3][110];
// dp[i][n] = min(dp[i][n-1],dp[3-i][n-1]+t[3-i][n-1])+a[i][n]
int bt[3][110];
int path[110];

int main() {
	scanf("%d%d%d%d%d", &n, &e1, &e2, &x1, &x2);

	for (int i = 1; i <= n; i++) scanf("%d", &a[1][i]);
	for (int i = 1; i <= n; i++) scanf("%d", &a[2][i]);
	for (int i = 1; i <= n - 1; i++) scanf("%d", &t[1][i]);
	for (int i = 1; i <= n - 1; i++) scanf("%d", &t[2][i]);

	dp[1][1] = e1 + a[1][1];
	dp[2][1] = e2 + a[2][1];

	for (int j = 2; j <= n; j++) {
		for (int i = 1; i <= 2; i++) {
			if (dp[i][j - 1] < dp[3 - i][j - 1] + t[3 - i][j - 1]) {
				dp[i][j] = dp[i][j - 1] + a[i][j];
				bt[i][j] = i;
			}
			else if (dp[i][j - 1] > dp[3 - i][j - 1] + t[3 - i][j - 1]) {
				dp[i][j] = dp[3 - i][j - 1] + t[3 - i][j - 1] + a[i][j];
				bt[i][j] = 3 - i;
			}
			else { // same -> first line
				dp[i][j] = dp[i][j - 1] + a[i][j];
				bt[i][j] = 1;
			}
		}
	}
	
	int start; // backtracking start
	if (dp[1][n] + x1 <= dp[2][n] + x2) {
		printf("%d\n", dp[1][n] + x1);
		start = 1;
	}
	else {
		printf("%d\n", dp[2][n] + x2);
		start = 2;
	}

	path[n] = start;
	for (int i = n - 1; i >= 1; i--) {
		path[i] = bt[path[i + 1]][i + 1];
	}

	for (int i = 1; i <= n; i++) {
		printf("%d %d\n", path[i], i);
	}

	return 0;
}