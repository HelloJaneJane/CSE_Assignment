// 2017030137_¿Ã¡¶¿Œ_A

#include <stdio.h>
#include <queue>
using namespace std;

int n;
int root[1010];
int rank[1010];

int find(int x) {
	if (x == root[x]) return x;
	return root[x] = find(root[x]);
}

priority_queue< pair<int, pair<int, int> > > pq;
priority_queue< pair<int, pair<int, int> > > ans;

void swap(int &a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int main() {
	scanf("%d", &n);

	for (int i = 1; i <= n; i++) root[i] = i;

	int x, y, w;
	while (scanf("%d%d%d", &x, &y, &w) != EOF) {
		pq.push({ -w,{-x,-y} });
	}

	while (!pq.empty()) {
		w = -pq.top().first;
		x = -pq.top().second.first;
		y = -pq.top().second.second;

		if (find(x) != find(y)) {
			root[find(x)] = find(y);
			ans.push({ -w,{-x,-y} });
		}
		pq.pop();
	}

	printf("%d\n", ans.size());
	while (!ans.empty()) {
		w = -ans.top().first;
		x = -ans.top().second.first;
		y = -ans.top().second.second;
		if (x > y) swap(x, y);
		printf("%d %d %d\n", x, y, w);
		ans.pop();
	}

	return 0;
}