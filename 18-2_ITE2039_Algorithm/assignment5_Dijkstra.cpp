#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define INF 987654321

int n;
vector< pair<int, int> > graph[5010];
int dis[5010];

priority_queue< pair<int, int> > pq; // { -w, v}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		int a, e;
		scanf("%d%d", &a, &e);
		if (a != 1) dis[a] = INF;
		while (e--) {
			int v, w;
			scanf("%d%d", &v, &w);
			graph[a].push_back({ v,w });
		}
	}

	pq.push({ 0,1 });
	while (!pq.empty()) {
		int nowW = -pq.top().first;
		int nowV = pq.top().second;
		pq.pop();

		if (dis[nowV] >= nowW) {
			for (int i = 0; i < graph[nowV].size(); i++) {
				int toV = graph[nowV][i].first;
				int toW = nowW + graph[nowV][i].second;
				if (dis[toV] > toW) {
					dis[toV] = toW;
					pq.push({ -toW,toV });
				}
			}
		}
	}

	int ans = 0;
	for (int i = 2; i <= n; i++) {
		if (ans < dis[i]) ans = dis[i];
	}
	printf("%d", ans);
	return 0;
}