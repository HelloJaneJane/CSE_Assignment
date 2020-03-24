#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define Max 200

int N;
char path[Max];

// 입력받은 절대경로가 저장되어 있는 path 뒤에 '\result(N).txt'를 추가
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

// 답을 출력할 파일
FILE *f;
// 파일 출력 시작, 끝 함수
void st_print() { f = fopen(path, "w"); }
void fin_print() { fclose(f); }

// ‘DFS, BFS, DFID Search’ 방법에 대한 결과물 출력 (각 방법에서 첫 번째로 찾아진 답)
void result_print(char* name, int solv, int* loc, double time) {
	// name : 탐색 방법 이름
	// solv : 답이 없으면 -1, 있으면 1	/ 0이면 에러
	// loc : 0~(N-1)의 Column에 있는 각 Queen의 row 위치
	// time : 탐색하는 데 걸린 시간

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

// 현재 배치된 N개의 Queen들이 서로 공격하지 못하는 위치에 있는지 판단
int isok(int N, int chk[]) {
	// chk배열은 0~N-1 다 채워진 상태여야 함 -> -1이 들어있으면 0 리턴
	for (int i = 0; i < N; i++) {
		if (chk[i] == -1) return 0;
	}

	// 공격 가능하면 0, 아니면 1 return
	for (int l = 0; l < N - 1; l++) {
		for (int r = l + 1; r < N; r++) {

			// left의 위치와 right의 위치가 같으면 가로로 공격 가능
			if (chk[l] == chk[r]) return 0;

			// left의 위치와 right의 위치의 차가 left~right 차이와 같으면 대각선으로 공격 가능
			int d = chk[l] - chk[r];
			if (d < 0) d *= -1;
			if (d == (r - l)) return 0;
		}
	}

	// 여기까지 다 통과했으면 만족하는 답이므로 1을 return한다
	return 1;
}


/////////////////////////////
// 1. DFS

int islast(int N, int chk[]);
void DFS(int N, int cnt, int now[], double start);
int ret;


/////////////////////////////
// 2. BFS

// 연결리스트로 큐 구현
typedef struct Node {
	int* data;
	struct Node* next;
}node;
typedef struct Queue {
	node* front;
	node* rear;
	int cnt; // 현재 큐에 들어있는 노드 수
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

	modify_path(); // 입력받은 정보로 결과 파일 저장할 경로 수정

	st_print(); // 출력 시작

	DFS(N, 0, NULL, 0);

	DFID(N);

	BFS(N);
	fin_print(); // 출력 끝

	return 0;
}





// 위치배열이 모두 N-1이면 맨 마지막 노드이므로 1을 리턴
int islast(int N, int chk[]) {
	for (int i = 0; i < N; i++) {
		if (chk[i] != N - 1) return 0;
	}
	return 1;
}

// 첫 번째 답이 구해지면 ret을 1로 바꾼다
int ret = 0;

// Depth First Search : 깊이 우선 탐색
void DFS(int N, int cnt, int now[], double start) {

	// 답이 나온 이후에 호출되는 함수는 실행하지 않음
	if (ret == 1) {
		free(now);
		return;
	}

	// DFS가 가장 처음 호출될 때 초기화 
	if (cnt == 0) {
		start = clock();
		now = (int*)malloc(sizeof(int)*N);
		for (int i = 0; i < N; i++) now[i] = -1;
	}

	// 현재까지 퀸을 cnt개 배치함 -> cnt번 column에 배치할 차례

	// 배치된 퀸 개수가 N이면 isok 판단
	if (cnt == N) {
		// 답을 찾으면 결과 출력 후 함수 끝내기
		if (isok(N, now)) {
			result_print("DFS", 1, now, (double)clock() - start);
			ret = 1;
			free(now);
			return;
		}
		// 못 찾았는데 지금 보고있는 게 마지막이면 답 없음
		else {
			if (islast(N, now)) {
				result_print("DFS", -1, NULL, (double)clock() - start);
				free(now);
			}
		}
	}

	// cnt번 column에 퀸을 배치 (0~N-1번 row)
	else {
		for (int i = 0; i < N; i++) {
			int* nxt = (int*)malloc(sizeof(int)*N);

			// 메모리초과 에러
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


// Breadth First Search : 너비 우선 탐색
void BFS(int N) {
	
	double start = clock();

	Q qu;
	Q_init(&qu);

	int* first = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++) first[i] = -1;
	Q_push(&qu, first);
	free(first);

	while (!Q_isempty(&qu)) {
		// 현재 체스판 now의 num번째 줄에 퀸을 배치해야함

		// qu를 pop해서 now에 저장, num값 찾아내기
		int* now = (int*)malloc(sizeof(int)*N);
		// 메모리초과 에러
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
		
		// N-1번째 줄까지 퀸이 모두 배치되었으면 isok 판단 
		if (num == N) {
			// 답을 찾으면 결과 출력 후 함수 끝내기
			if (isok(N, now)) {
				result_print("BFS", 1, now, (double)clock() - start);
				free(now);

				while (!Q_isempty(&qu)) free(Q_pop(&qu));
				return;
			}
			// 못 찾으면 계속 진행하기
			else {
				free(now);
				continue;
			}
		}

		// 퀸이 덜 배치되었으면 num번 column에 퀸을 배치한 (0번row ~ N-1번row) 후 qu에 넣고 반복
		else {
			for (int i = 0; i < N; i++) {
				// qu에 새로 넣을 배열 nxt를 만듦
				int* nxt = (int*)malloc(sizeof(int)*N);
				// 메모리초과 에러
				if (nxt == NULL) {
					result_print("DFID", 0, NULL, 0);
					return;
				}
				// now 복사 후 num번 column만 수정
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

	// qu가 빌 때까지 return하지 못 했으면 답이 없음
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

	if (now == NULL) return -1; // 메모리가 부족해 now에 할당이 되지 않았으므로 error 처리

	now->data = (int*)malloc(sizeof(int)*N);
	for (int k = 0; k < N; k++) now->data[k] = data[k];
	now->next = NULL;

	// 큐가 비어있으면 맨 앞이 now
	if (Q_isempty(qu)) qu->front = now;//맨 앞을 now로 설정       
	// 큐가 안 비어있으면 맨 뒤의 다음이 now
	else qu->rear->next = now;

	qu->rear = now;
	qu->cnt++;
	
	// 에러 발생하지 않으면 정상적으로 1 리턴
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


// DFS - Iterative Deepening Search : 점차적 깊이 제한 탐색
void DFID(int N) {
	double start = clock();

	// 현재 체스판 now
	int* now = (int*)malloc(sizeof(int)*N);

	// 첫 번째 답이 구해지면 ret을 1로 바꾼다
	rret = 0;

	// depth : 트리 깊이(루트깊이=0 으로 계산) && 현재 배치된 퀸 개수
	for (int depth = 0; depth <= N; depth++) {
		//printf("\ndepth:%d", depth);

		// depth(limit) 새로 시작할 때마다 초기화
		for (int i = 0; i < N; i++) now[i] = -1;

		// DLS 결과 값을 찾으면 return
		DLS(N, 0, depth, now, start);
	}

	// depth를 끝까지 탐색할 때까지 return하지 못 했으면 답이 없음
	if (rret == 0) result_print("DFID", -1, NULL, (double)clock() - start);
}

// Depth Limited Search : 깊이 제한 탐색
void DLS(int N, int cnt, int limit, int now[], double start) {

	// 답이 나온 이후에 호출되는 함수는 실행하지 않음
	if (rret == 1) {
		free(now);
		return;
	}

	// cnt : 현재까지 배치한 퀸 개수
	// limit : 제한 깊이 (cnt는 0부터 limit까지만 가능)

	// 깊이가 0이면 바로 return
	if (limit == 0) {
		free(now);
		return;
	}

	// 깊이가 N이면 답인지 확인하기
	else if (cnt == N && limit == N) {
		// 답을 찾으면 결과 출력 후 함수 끝내기
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

	// 그 외에는 dfs돌리기
	else {
		// cnt번 column에 배치할 차례 (0~N-1번 row)
		// 다음 cnt가 limit보다 작거나 같을 때만 재귀 더 진행
		if (cnt + 1 <= limit) {
			for (int i = 0; i < N; i++) {
				int* nxt = (int*)malloc(sizeof(int)*N);

				// 메모리초과 에러
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

