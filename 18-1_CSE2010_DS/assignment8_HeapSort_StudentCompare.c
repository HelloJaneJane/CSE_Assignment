#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ELEMENTS 100000
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
	char name[100];
	int kor;
	int eng;
	int math;
} student;

student heap[MAX_ELEMENTS];
int n = 0;

int studentCmp(student stu1, student stu2) { // 왼쪽이면 -1, 오른쪽이면 1 리턴
	if (stu1.kor > stu2.kor) return -1;
	else if (stu1.kor < stu2.kor) return 1;
	else { // 국어 점수 같을 때
		if (stu1.eng < stu2.eng) return -1;
		else if (stu1.eng > stu2.eng) return 1;
		else { // 영어 점수 같을 때
			if (stu1.math > stu2.math) return -1;
			else if (stu1.math < stu2.math) return 1;
			else { // 수학 점수 같을 때
				return strcmp(stu1.name, stu2.name);
			}
		}
	}
}
void adjust(int root, int n) {
	int child;
	student rootStu;
	rootStu = heap[root];
	child = 2 * root; // left child
	while (child <= n) {
		if ((child < n) && (studentCmp(heap[child], heap[child + 1]) > 0)) {
			child += 1;
		}
		if (studentCmp(rootStu, heap[child]) < 0) {
			break;
		}
		else {
			heap[child / 2] = heap[child];
			child *= 2;
		}
	}
	heap[child / 2] = rootStu;
}
void mHeapsort() {
	int i, j;
	student tmp;
	for (i = n / 2; i > 0; i--) adjust(i, n);
	for (i = n - 1; i > 0; i--) {
		// swap
		tmp = heap[1];
		heap[1] = heap[i + 1];
		heap[i + 1] = tmp;

		adjust(1, i);
	}

}

int main() {
	int num = 0;
	int i = 0;
	int tmpK, tmpE, tmpM;
	char tmpN[100];
	scanf("%d", &num);

	for (i = 0; i < num; i++) {
		n++;
		scanf("%s %d%d%d", tmpN, &tmpK, &tmpE, &tmpM);
		heap[n].kor = tmpK;
		heap[n].eng = tmpE;
		heap[n].math = tmpM;
		strcpy(heap[n].name, tmpN);
	}
	mHeapsort();

	for (i = n; i > 0; i--) {
		printf("%s\n", heap[i].name);
	}
	return 0;
}