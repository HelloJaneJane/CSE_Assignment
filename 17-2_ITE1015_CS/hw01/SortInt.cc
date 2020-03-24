/*
1-4. 정수 정렬

입력받은 정수 배열을 정렬
 ● 입력받은 수의 정수형 배열을 동적할당하고 배열 값을 입력받은 후
 직접 작성한 정렬 함수를 호출
 ● 개수에 0이나 음수가 입력되면 아무 출력없이 종료
*/

#include <iostream>
using namespace std;

// 삽입 정렬
void sort(int arr[], int n){
	for (int i = 1; i < n; i++) {
		int tmp = arr[i];
		int j = i - 1;
		while (j >= 0 && tmp < arr[j]) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
	}
}

int main(){
	int n;
	cin >> n;
	if (n <= 0) return 0;
	else {
		int* arr = new int[n];
				
		for (int i = 0; i < n; i++) {
			cin >> arr[i];
		}
		sort(arr, n);
		for (int i = 0; i < n; i++) {
			cout << arr[i] << " ";
		}
	}
	return 0;
}

