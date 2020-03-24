#include <stdio.h>

int n, m, v;
int node[1010][1010]; // 1이 들어있으면 row랑 col은 연결되어 있음
int visit[1010];

int q[10000];
int front = -1;
int tail = -1;

void enqueue(int a) {
	q[++tail] = a;
}

int isempty() {
	return (front == tail ? 1 : 0);
}

int dequeue() {
	return q[++front];
}

void dfs(int now) {
	visit[now] = 1;
	printf("%d ", now);
	for (int j = 1; j <= n; j++) {
		int next = j;
		if (node[now][next] == 1 && visit[next] == 0) {
				dfs(next);
		}
	}
}

void bfs(int start) {
	enqueue(start);
	visit[start] = 1;

	while (!isempty()) {
		int now = dequeue();
		printf("%d ", now);
		for (int j = 1; j <= n; j++) {
			int next = j;
			if (node[now][next] == 1 && visit[next] == 0) {
				visit[next] = 1;
				enqueue(next);
			}
		}
	}
}

int main() {
	scanf("%d%d%d", &n, &m, &v);
	for (int i = 0; i < m; i++) {
		int row, col;
		scanf("%d%d", &row, &col);
		node[row][col] = node[col][row] = 1;
	}
	dfs(v);
	printf("\n");
	for (int i = 1; i <= n; i++) visit[i] = 0;
	bfs(v);
	return 0;
}