/*
1-2. 피보나치 수열

입력받은 정수의 개수만큼 피보나치 수열 출력
 ● Recursion 을 이용하여 작성하고 결과값도 recursive function 내에서 직접 출력
 ● 음수나 짝수등 잘못된 입력이면 아무 출력없이 종료
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
       
