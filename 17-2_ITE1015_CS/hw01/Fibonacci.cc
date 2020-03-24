/*
1-2. �Ǻ���ġ ����

�Է¹��� ������ ������ŭ �Ǻ���ġ ���� ���
 �� Recursion �� �̿��Ͽ� �ۼ��ϰ� ������� recursive function ������ ���� ���
 �� ������ ¦���� �߸��� �Է��̸� �ƹ� ��¾��� ����
*/

#include <iostream>
using namespace std;

int n;
int* dp = new int[n];

int fibo(int i){

	if (i == 1 || i == 2) {
		dp[i] = 1;
		cout << "1 ";
	}
	else {
		dp[i] = dp[i - 1] + dp[i - 2];
		cout << dp[i] << " ";
	}

	if (i < n) return dp[i + 1] = fibo(i + 1);
	else return 1;
}

int main(){
	cin >> n;
       
	if (n <= 0) return 0;
	else fibo(1);

    return 0;
}
       
