#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int dp[505][505];
int map[505][505]; // 1 : - , 2 : | , 3 : \	3인거를 출력해야 LCS

int main() {
	string a, b;
	cin >> a >> b;

	int an = a.length();
	int bn = b.length();

	if (an > bn) {
		string tmp = a;
		a = b;
		b = tmp;
		an = a.length();
		bn = b.length();
	}

	for (int i = 1; i <= an; i++) {
		for (int j = 1; j <= bn; j++) {
			if (a[i - 1] == b[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
				map[i][j] = 3;
			}
			else {
				if (dp[i - 1][j] >= dp[i][j - 1]) {
					dp[i][j] = dp[i - 1][j];
					map[i][j] = 2;
				}
				else {
					dp[i][j] = dp[i][j - 1];
					map[i][j] = 1;
				}
			}
		}
	}

	string ans;
	int i = an;
	int j = bn;
	while (i > 0 && j > 0) {
		if (map[i][j] == 3) {
			ans += a[i - 1];
			i--;
			j--;
		}
		else if (map[i][j] == 2) i--;
		else if (map[i][j] == 1) j--;
	}

	int l = ans.length();
	for (int i = 1; i <= l; i++) {
		cout << ans[l - i];
	}
	return 0;

}