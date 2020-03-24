// 2017030137_¿Ã¡¶¿Œ_A

#include <stdio.h>

int n;
int root[1010];
int rank[1010];

int find(int x) {
	if (x != root[x]) root[x] = find(root[x]);
	return root[x];
}

void link(int a, int b) {
	if (rank[a] > rank[b]) root[b] = a;
	else {
		root[a] = b;
		if (rank[a] == rank[b]) rank[b]++;
	}
}

int main() {
	scanf("%d", &n);

	for (int i = 1; i <= n; i++) root[i] = i;

	int a, b;
	while (scanf("%d%d", &a, &b) != EOF) {
		a = find(a);
		b = find(b);
		if (a != b) link(a, b);
	}

	for (int i = 1; i <= n; i++) root[i] = find(i);

	int cnt = 0;
	for (int i = 1; i <= n; i++) {
		if (cnt < root[i]) {
			cnt++;
			int save = root[i];
			for (int j = i; j <= n; j++)
				if (root[j] == save) root[j] = cnt;
		}
	}
	printf("%d\n", cnt);
	for (int i = 1; i <= n; i++) printf("%d\n", root[i]);

	return 0;
}