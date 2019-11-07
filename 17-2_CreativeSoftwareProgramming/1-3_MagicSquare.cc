/*
1-3. 마방진

3 이상의 홀수 n을 입력 받으면 n*n 크기의 마방진 출력

** 마방진 만들기 (위키 출처) **
첫 번째 행의 가운데 칸에 1을 넣는다.
이어서 다음과 같은 규칙으로 숫자를 채운다.
다음 숫자를 대각선 방향으로 오른쪽 위 칸에 넣는다.
이때 해당하는 칸이 마방진의 위쪽으로 벗어난 경우에는 반대로 가장 아래쪽의 칸으로,
마방진의 오른쪽으로 벗어나는 경우는 가장 왼쪽의 칸으로 각자 한번 더 이동한다.
그리고 다른 수가 오른쪽 위 칸에 있어도 경우에 따라 수직 또는 수평으로 이동한다.


내가 만든 건
-----3-----
4 9 2
3 5 7
8 1 6
-----5-----
11 18 25  2  9
10 12 19 21  3
 4  6 13 20 22
23  5  7 14 16
17 24  1  8 15


*/



#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int main() {
	int n;
	cin >> n;

	// 음수나 짝수 입력 받으면 종료
	if (n <= 0 || n % 2 == 0) return 0;

	// 동적할당
	int** arr = new int*[n];
	for (int i = 0; i < n; i++) {
		arr[i] = new int[n];
	}

	// 0으로 초기화
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			arr[i][j] = 0;
		}
	}

	// 맨 처음에 넣는 곳 : 맨 밑 row, 가운데 col
	int c = (n - 1) / 2;
	int r = n - 1;

	// arr[r][c]에 cnt를 넣는다
	for (int cnt = 1; cnt <= n * n; cnt++) {
		// row col 둘 다 범위 벗어나면 그 전(r=n-1, c=n-1)의 위에로
		if (r == n && c == n) {
			c--;
			r = r - 2;
		}
		// row 범위 벗어나면 맨 위로
		else if (r == n && c != n) r = 0;
		// col 범위 벗어나면 맨 왼쪽으로
		else if (c == n && r != n) c = 0;

		// cnt넣고 다음 r,c를 내 오른쪽 대각선 아래로 설정
		if (arr[r][c] == 0) {
			// 내 자리 비어있으면 그냥 넣기
			arr[r][c] = cnt;
			r++;
			c++;
		}
		else {
			// 내 자리에 다른 수가 있으면 그 전(r=n-1, c=n-1)의 위에로
			c--;
			r = r - 2;
			arr[r][c] = cnt;
			r++;
			c++;
		}
	}

	// 출력
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
