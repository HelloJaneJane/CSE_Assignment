#include <stdio.h>

int n, m;
int num[100010];
int cnt;

int main() {
	scanf("%d%d", &n, &m);
	while (n--) {
		int a;
		scanf("%d", &a);
		num[a]++;
	}
	while (m--) {
		int b;
		scanf("%d", &b);
		if (num[b]) cnt++;
	}
	printf("%d", cnt);
	return 0;
}