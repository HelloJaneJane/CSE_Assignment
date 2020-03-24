#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define Max 200

int N;
char path[Max];

// �Է¹��� �����ΰ� ����Ǿ� �ִ� path �ڿ� '\result(N).txt'�� �߰�
void modify_path() {

	char* path_p = path;
	while (*path_p != '\0') path_p++;

	*(path_p++) = '\\';

	char name[10] = "result";
	char* name_p = name;
	while (*name_p != '\0') { *(path_p++) = *(name_p++); }

	char num[5];
	sprintf(num, "%d", N);
	char* num_p = num;
	while (*num_p != '\0') { *(path_p++) = *(num_p++); }

	char typ[5] = ".txt";
	char* typ_p = typ;
	while (*typ_p != '\0') { *(path_p++) = *(typ_p++); }

	*path_p = '\0';
}

// ���� ����� ����
FILE *f;
// ���� ��� ����, �� �Լ�
void st_print() { f = fopen(path, "w"); }
void fin_print() { fclose(f); }

// ��DFS, BFS, DFID Search�� ����� ���� ����� ��� (�� ������� ù ��°�� ã���� ��)
void result_print(char* name, int solv, int* loc, double time) {
	// name : Ž�� ��� �̸�
	// solv : ���� ������ -1, ������ 1	/ 0�̸� ����
	// loc : 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
	// time : Ž���ϴ� �� �ɸ� �ð�

	fprintf(f, ">%s\n", name);

	if (solv == -1) fprintf(f, "No solution\n");
	else if (solv == 0) {
		fprintf(f, "Memory Limit Error\n\n");
		return;
	}
	else {
		fprintf(f, "Location :");
		for (int i = 0; i < N; i++) fprintf(f, " %d", loc[i]);
		fprintf(f, "\n");
	}

	fprintf(f, "Time : %lf\n\n", time);
}

// ���� ��ġ�� N���� Queen���� ���� �������� ���ϴ� ��ġ�� �ִ��� �Ǵ�
int isok(int N, int chk[]) {
	// chk�迭�� 0~N-1 �� ä���� ���¿��� �� -> -1�� ��������� 0 ����
	for (int i = 0; i < N; i++) {
		if (chk[i] == -1) return 0;
	}

	// ���� �����ϸ� 0, �ƴϸ� 1 return
	for (int l = 0; l < N - 1; l++) {
		for (int r = l + 1; r < N; r++) {

			// left�� ��ġ�� right�� ��ġ�� ������ ���η� ���� ����
			if (chk[l] == chk[r]) return 0;

			// left�� ��ġ�� right�� ��ġ�� ���� left~right ���̿� ������ �밢������ ���� ����
			int d = chk[l] - chk[r];
			if (d < 0) d *= -1;
			if (d == (r - l)) return 0;
		}
	}

	// ������� �� ��������� �����ϴ� ���̹Ƿ� 1�� return�Ѵ�
	return 1;
}


/////////////////////////////
// 1. DFS

int islast(int N, int chk[]);
void DFS(int N, int cnt, int now[], double start);
int ret;


/////////////////////////////
// 2. BFS

// ���Ḯ��Ʈ�� ť ����
typedef struct Node {
	int* data;
	struct Node* next;
}node;
typedef struct Queue {
	node* front;
	node* rear;
	int cnt; // ���� ť�� ����ִ� ��� ��
}Q;

void Q_init(Q* qu);
int Q_isempty(Q* qu);
int Q_push(Q* qu, int data[]);
int* Q_pop(Q* qu);

void BFS(int N);


//////////////////////////////
// 3. DFID
void DFID(int N);
void DLS(int N, int cnt, int limit, int now[], double start);
int rret;

//////////////////////////////
// main
int main() {
	scanf("%d", &N);
	scanf("%s", path);

	modify_path(); // �Է¹��� ������ ��� ���� ������ ��� ����

	st_print(); // ��� ����

	DFS(N, 0, NULL, 0);

	DFID(N);

	BFS(N);
	fin_print(); // ��� ��

	return 0;
}





// ��ġ�迭�� ��� N-1�̸� �� ������ ����̹Ƿ� 1�� ����
int islast(int N, int chk[]) {
	for (int i = 0; i < N; i++) {
		if (chk[i] != N - 1) return 0;
	}
	return 1;
}

// ù ��° ���� �������� ret�� 1�� �ٲ۴�
int ret = 0;

// Depth First Search : ���� �켱 Ž��
void DFS(int N, int cnt, int now[], double start) {

	// ���� ���� ���Ŀ� ȣ��Ǵ� �Լ��� �������� ����
	if (ret == 1) {
		free(now);
		return;
	}

	// DFS�� ���� ó�� ȣ��� �� �ʱ�ȭ 
	if (cnt == 0) {
		start = clock();
		now = (int*)malloc(sizeof(int)*N);
		for (int i = 0; i < N; i++) now[i] = -1;
	}

	// ������� ���� cnt�� ��ġ�� -> cnt�� column�� ��ġ�� ����

	// ��ġ�� �� ������ N�̸� isok �Ǵ�
	if (cnt == N) {
		// ���� ã���� ��� ��� �� �Լ� ������
		if (isok(N, now)) {
			result_print("DFS", 1, now, (double)clock() - start);
			ret = 1;
			free(now);
			return;
		}
		// �� ã�Ҵµ� ���� �����ִ� �� �������̸� �� ����
		else {
			if (islast(N, now)) {
				result_print("DFS", -1, NULL, (double)clock() - start);
				free(now);
			}
		}
	}

	// cnt�� column�� ���� ��ġ (0~N-1�� row)
	else {
		for (int i = 0; i < N; i++) {
			int* nxt = (int*)malloc(sizeof(int)*N);

			// �޸��ʰ� ����
			if (nxt == NULL) {
				result_print("DFID", 0, NULL, 0);
				rret = 1;
				return;
			}
			for (int k = 0; k < N; k++) nxt[k] = now[k];
			nxt[cnt] = i;
			DFS(N, cnt + 1, nxt, start);
		}
	}
}


// Breadth First Search : �ʺ� �켱 Ž��
void BFS(int N) {
	
	double start = clock();

	Q qu;
	Q_init(&qu);

	int* first = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) first[i] = -1;
	Q_push(&qu, first);
	free(first);

	while (!Q_isempty(&qu)) {
		// ���� ü���� now�� num��° �ٿ� ���� ��ġ�ؾ���

		// qu�� pop�ؼ� now�� ����, num�� ã�Ƴ���
		int* now = (int*)malloc(sizeof(int)*N);
		// �޸��ʰ� ����
		if (now == NULL) {
			result_print("DFID", 0, NULL, 0);
			return;
		}
		now = Q_pop(&qu);
		int num;
		for (int i = 0; i < N; i++) {
			if (now[i] == -1) {
				num = i; break;
			}
			if (i == N - 1) num = N;
		}
		
		// N-1��° �ٱ��� ���� ��� ��ġ�Ǿ����� isok �Ǵ� 
		if (num == N) {
			// ���� ã���� ��� ��� �� �Լ� ������
			if (isok(N, now)) {
				result_print("BFS", 1, now, (double)clock() - start);
				free(now);

				while (!Q_isempty(&qu)) free(Q_pop(&qu));
				return;
			}
			// �� ã���� ��� �����ϱ�
			else {
				free(now);
				continue;
			}
		}

		// ���� �� ��ġ�Ǿ����� num�� column�� ���� ��ġ�� (0��row ~ N-1��row) �� qu�� �ְ� �ݺ�
		else {
			for (int i = 0; i < N; i++) {
				// qu�� ���� ���� �迭 nxt�� ����
				int* nxt = (int*)malloc(sizeof(int)*N);
				// �޸��ʰ� ����
				if (nxt == NULL) {
					result_print("DFID", 0, NULL, 0);
					return;
				}
				// now ���� �� num�� column�� ����
				for (int k = 0; k < N; k++) nxt[k] = now[k];
				nxt[num] = i;
				int flag = Q_push(&qu, nxt);
				if (flag == -1) {
					result_print("BFS", 0, NULL, 0);
					return;
				}
				free(nxt);
			}
			free(now);
		}
	}

	// qu�� �� ������ return���� �� ������ ���� ����
	result_print("BFS", -1, NULL, (double)clock() - start);
	return;
}

void Q_init(Q* qu) {
	qu->front = qu->rear = NULL;
	qu->cnt = 0;
}
int Q_isempty(Q* qu) {
	return (qu->cnt == 0);
}
int Q_push(Q* qu, int data[]) {
	node* now = (node*)malloc(sizeof(node));

	if (now == NULL) return -1; // �޸𸮰� ������ now�� �Ҵ��� ���� �ʾ����Ƿ� error ó��

	now->data = (int*)malloc(sizeof(int)*N);
	for (int k = 0; k < N; k++) now->data[k] = data[k];
	now->next = NULL;

	// ť�� ��������� �� ���� now
	if (Q_isempty(qu)) qu->front = now;//�� ���� now�� ����       
	// ť�� �� ��������� �� ���� ������ now
	else qu->rear->next = now;

	qu->rear = now;
	qu->cnt++;
	
	// ���� �߻����� ������ ���������� 1 ����
	return 1;
}
int* Q_pop(Q *qu) {

	if (Q_isempty(qu)) return NULL;

	node* save;
	save = qu->front;

	int* ret = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) ret[i] = save->data[i];
	free(save->data);

	qu->front = save->next;
	qu->cnt--;

	free(save);
	return ret;
}


// DFS - Iterative Deepening Search : ������ ���� ���� Ž��
void DFID(int N) {
	double start = clock();

	// ���� ü���� now
	int* now = (int*)malloc(sizeof(int)*N);

	// ù ��° ���� �������� ret�� 1�� �ٲ۴�
	rret = 0;

	// depth : Ʈ�� ����(��Ʈ����=0 ���� ���) && ���� ��ġ�� �� ����
	for (int depth = 0; depth <= N; depth++) {
		//printf("\ndepth:%d", depth);

		// depth(limit) ���� ������ ������ �ʱ�ȭ
		for (int i = 0; i < N; i++) now[i] = -1;

		// DLS ��� ���� ã���� return
		DLS(N, 0, depth, now, start);
	}

	// depth�� ������ Ž���� ������ return���� �� ������ ���� ����
	if (rret == 0) result_print("DFID", -1, NULL, (double)clock() - start);
}

// Depth Limited Search : ���� ���� Ž��
void DLS(int N, int cnt, int limit, int now[], double start) {

	// ���� ���� ���Ŀ� ȣ��Ǵ� �Լ��� �������� ����
	if (rret == 1) {
		free(now);
		return;
	}

	// cnt : ������� ��ġ�� �� ����
	// limit : ���� ���� (cnt�� 0���� limit������ ����)

	// ���̰� 0�̸� �ٷ� return
	if (limit == 0) {
		free(now);
		return;
	}

	// ���̰� N�̸� ������ Ȯ���ϱ�
	else if (cnt == N && limit == N) {
		// ���� ã���� ��� ��� �� �Լ� ������
		if (isok(N, now)) {
			result_print("DFID", 1, now, (double)clock() - start);
			free(now);
			rret = 1;
			return;
		}
		else {
			free(now);
			return;
		}
	}

	// �� �ܿ��� dfs������
	else {
		// cnt�� column�� ��ġ�� ���� (0~N-1�� row)
		// ���� cnt�� limit���� �۰ų� ���� ���� ��� �� ����
		if (cnt + 1 <= limit) {
			for (int i = 0; i < N; i++) {
				int* nxt = (int*)malloc(sizeof(int)*N);

				// �޸��ʰ� ����
				if (nxt == NULL) {
					result_print("DFID", 0, NULL, 0);
					rret = 1;
					return;
				}
				for (int k = 0; k < N; k++) nxt[k] = now[k];
				nxt[cnt] = i;
				DLS(N, cnt + 1, limit, nxt, start);
			}
		}
		else {
			free(now);
			return;
		}
	}

}

