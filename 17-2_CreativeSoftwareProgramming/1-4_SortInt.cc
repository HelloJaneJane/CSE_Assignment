/*
1-4. ���� ����

�Է¹��� ���� �迭�� ����
 �� �Է¹��� ���� ������ �迭�� �����Ҵ��ϰ� �迭 ���� �Է¹��� ��
 ���� �ۼ��� ���� �Լ��� ȣ��
 �� ������ 0�̳� ������ �ԷµǸ� �ƹ� ��¾��� ����
*/

#include <iostream>
using namespace std;

// ���� ����
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

