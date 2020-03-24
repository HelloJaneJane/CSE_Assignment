#include <stdio.h>

int N;
int arr[30010];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) scanf("%d", &arr[i]);

	for (int i = 1; i < N; i++) {
		int tmp = arr[i];
		int j = i - 1;
		while (j >= 0 && tmp < arr[j]) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
	}

	for (int i = N - 1; i >= 0; i--) printf("%d\n", arr[i]);
	
	return 0;
}