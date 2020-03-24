#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

int n;

vector<int> graph[1010];
//int ind[1010];
vector<int> ans;

int visit[1010]; // 0:���� 1:�湮�� 2:�湮�Ϸ�

bool cycle = 0;

void dfs(int now) {
	visit[now] = 1;
	for (int i = 0; i < graph[now].size(); i++) {
		if (visit[graph[now][i]] == 1) {
			cycle = 1;
			return;
		}
		else if (visit[graph[now][i]] == 0) dfs(graph[now][i]);
	}
	visit[now] = 2;
	ans.push_back(now);
}


int main() {
	scanf("%d", &n);
	int u, v;
	while (scanf("%d%d", &u, &v) != EOF) {
		graph[u].push_back(v);
		//ind[v]++;
	}

	for (int i = 1; i <= n; i++) sort(graph[i].begin(), graph[i].end());

	for (int i = 1; i <= n; i++) {
		if (visit[i] == 0) {
			dfs(i);
		}
	}
	
	if (cycle) printf("0");
	else {
		printf("1\n");
		for (int i = n - 1; i >= 0; i--) printf("%d ", ans[i]);
	}

	return 0;
}
