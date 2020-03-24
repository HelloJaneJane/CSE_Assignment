#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct stack *Stack;

struct stack {
	char * array;
	int size;
	int top;
};

Stack createStack(int arraySize) {
	Stack S = (Stack)malloc(sizeof(struct stack));
	S->size = arraySize;
	S->top = -1;
	S->array = (char *)malloc(sizeof(int) * arraySize);
	return S;
}

Stack makeEmptyStack(Stack S) { // data만 비우고 memory는 아직
	S->top = -1;
	for (int i = 0; i < S->size; i++) S->array[i] = '0';
	return S;
}

int isEmpty(Stack S) { // 1이면 empty
	if (S->top == -1) return 1;
	return 0;
}

void push(char X, Stack S) { // 원소 X를 스택 S에 넣는다
	S->array[++(S->top)] = X;
}

char pop(Stack S) { // 스택에 가장 최근에 들어간 원소 제거 & 리턴
	if (isEmpty(S)) return '0';
	return S->array[(S->top)--];
}
void deleteStack(Stack S) {
	makeEmptyStack(S);
	free(S);
}

char print[100];
int psize, fin;
Stack S;

int main() {
	S = createStack(100);
	while (1) {
		char input;
		scanf(" %c", &input);
		if (input == '!') break;
		
		if (input == '#') {
			if (isEmpty(S) && fin == 0) printf("right. ");
			else printf("wrong. ");

			for (int i = 0; i < psize; i++) {
				printf("%c ", print[i]);
			}
			printf("\n");

			psize = 0;
			S = createStack(100);
			fin = 0;
		}
		else {
			if (fin == 1) continue;

			if (input == '(') push('(', S);
			else if (input == ')') {
				if (isEmpty(S)) {
					fin = 1;
					continue;
				}
				while (1) {
					char a = pop(S);
					if (a == '(') break;
					else if (a != '0') print[psize++] = a;
				}
			}
			else {
				if (isEmpty(S)) print[psize++] = input;
				else push(input, S);
			}
		}

	}
	return 0;
}
