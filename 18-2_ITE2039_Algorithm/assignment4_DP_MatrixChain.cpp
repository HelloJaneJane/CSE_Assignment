#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#define INF 987654321;
int n;

int p[110];			// dimension (matrix A_i : p[i-1] * p[i])
int mul[110][110];	// minimum number of scalar multiplications
	// mul[i][j] : A_i ~ A_j Çà·Ä°ö¿¡¼­ ¼ýÀÚ°ö¼À¿¬»ê ÃÖ¼Ò È½¼ö
int save[110][110];	// store optimal k
	// save[i][j] : A_i ~ A_j Çà·Ä°ö¿¡¼­ A_i~A_k / A_k+1~A_j°¡ ÃÖ¼Ò

string str;

void insrt(int i, int j) {
	size_t n = str.find(i + '0');
	str.insert(n, "(");
	n = str.find(j + '0') + 1;
	str.insert(n, ")");
}

void f(int i, int j) {
	if (j - i == 1) return;

	int k = save[i][j];

	if (k == i) {
		insrt(k + 1, j);
		f(k + 1, j);
	}
	else if (k == j - 1) {
		insrt(i, k);
		f(i, k);
	}
	else {
		insrt(i, k);
		insrt(k + 1, j);
		f(i, k);
		f(k + 1, j);
	}
}


int main() {
	scanf("%d", &n);
	for (int i = 0; i <= n; i++) scanf("%d", &p[i]);

	for (int i = n; i >= 1; i--) {
		for (int j = i + 1; j <= n; j++) {
			mul[i][j] = INF;
			for (int k = i; k < j; k++) {
				int tmp = mul[i][k] + mul[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (mul[i][j] > tmp) {
					mul[i][j] = tmp;
					save[i][j] = k;
				}
			}
		}
	}

	printf("%d\n", mul[1][n]);

	for (int i = 1; i <= n; i++) {
		str += i + '0';
	}

	f(1, n);

	cout << "(" << str << ")" << endl;
	return 0;
}