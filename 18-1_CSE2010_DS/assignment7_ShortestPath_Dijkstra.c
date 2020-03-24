#include <stdio.h>

#define INF 987654321

int n, m;
int s, e;

int node[1010][1010]; 
int dis[1010];
int visit[1010];

int dijkstra(int u, int v) {
	dis[u] = 0;
	for (int t = 1; t <= n; t++) {
		int min = INF;
		int min_v = u;
		for (int i = 1; i <= n; i++) {
			if (visit[i] == 0 && min > dis[i]) {
				min = dis[i];
				min_v = i;
			}
		}
		visit[min_v] = 1;

		for (int j = 1; j <= n; j++) {
			if (dis[j] > dis[min_v] + node[min_v][j])
				dis[j] = dis[min_v] + node[min_v][j];
		}
	}
	return dis[v];	
}


int main() {
	scanf("%d%d", &n, &m);

	for (int i = 1; i <= n; i++) {
		dis[i] = INF;
		for (int j = 1; j <= n; j++) node[i][j] = INF;
	}

	for (int i = 0; i < m; i++) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		if (w < node[u][v]) node[u][v] = w;
	}
	scanf("%d%d", &s, &e);

	int ans = dijkstra(s, e);

	printf("%d", ans);
}