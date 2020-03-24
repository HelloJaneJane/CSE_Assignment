# Assignment1 : N-Queens problem

* N개의 Queen이 서로 공격할 수 없도록 배치하는 방법을 DFS, BFS, DFID 세 가지 방법으로 탐색한다.
* 탐색하는 과정에서 만들어진 tree는 leaf에 닿을 때까지 모든 경우의 수를 expanding 한다.
* C로 구현하여 gcc 컴파일을 이용



#### 탐색방법

### 1. DFS

* 깊이우선탐색 방법으로, 처음엔 퀸이 하나도 배치가 안 된 상태의 체스판으로 시작한다
* 첫 번째 column에 퀸을 배치, 0~N-1번 각 row에 하나씩 배치해가며 새 체스판으로 재귀호출한다. 이렇게 호출된 함수들은 각각 두 번째 column에도 같은 방법을 배치, 재귀호출을 반복한다.
* int now[]와 int cnt를 넘겨주면서 진행된다. 각 함수가 이어서 호출하는 함수들은 now에 새 체스판을, cnt에 이전 값+1의 값을 갖는다.
* now배열에는 현재 해당 column에 퀸이 몇 번 row에 배치되어있는지가 저장되어있다. (그 column에 배치되어있지 않으면 -1로 초기화되어 있다.)
* cnt는 현재까지 배치된 퀸의 개수로, 호출된 함수가 실행될 때 cnt번 column에 퀸을 배치한다.
* cnt번 column에는 row번호 0~N-1 가 들어갈 수 있으므로 nxt배열을 새로 만들어 cnt번에는 새 번호를 부여, 나머지 번호는 now배열과 같게 한 후 재귀함수를 새로 호출한다. 

```c
int* nxt = (int*)malloc(sizeof(int)*N);
for (int k = 0; k < N; k++) nxt[k] = now[k];
nxt[cnt] = i;
DFS(N, cnt + 1, nxt, start);
```

* N개의 퀸이 모두 배치되어있다면 완성된 체스판이 안전한지 여부를 판단하여, 탐색의 종결 여부를 결정한다.
* 안전한 체스판이 탐색되면 이미 호출된 함수들은 실행하면 안 되므로 전역변수를 활용한다.

```c
int ret = 0;
...
void DFS{
    if (ret == 1) {
		free(now);
		return;
	}
....
```

* 완성된 체스판이 tree의 맨 마지막 node인데 안전하지 않다면 답이 존재하지 않는 것으로 판단한다.

### 2. DFID

* 깊이 제한을 둔 DFS (DLS)를 제한지점(limit)을 0부터 N까지 늘려가며 반복하는 탐색방법이다.

```c
// depth : 트리 깊이(루트깊이=0 으로 계산) && 현재 배치된 퀸 개수
for (int depth = 0; depth <= N; depth++) {
		for (int i = 0; i < N; i++) now[i] = -1;		// depth(limit) 새로 시작할 때마다 초기화
		DLS(N, 0, depth, now, start);		// DLS 결과 값을 찾으면 return
}
```

* DLS는 DFS와 비슷하게 진행되고, 다음 항목만 다르다.
 - DFS는 cnt가 N일 때 (배치된 퀸 개수가 N일 때) isok 판단을 하고, DLS는 cnt와 limit이 N일 때 판단을 한다.
 - DFS는 cnt가 N보다 작으면 다음 column에 퀸을 하나 더 배치해서 재귀를 반복하고, DLS는 cnt가 limit보다 작을 때만 재귀를 반복한다.

```c
...
else if (cnt == N && limit == N) {			// 깊이가 N이면 답인지 확인하기
		if (isok(N, now)) {			// 답을 찾으면 결과 출력 후 함수 끝내기
			result_print("DFID", 1, now, (double)clock() - start);
			rret = 1;
			return;
	....	
// 그 외에는 dfs돌리기
else {
		// cnt번 column에 배치할 차례 (0~N-1번 row)
		if (cnt + 1 <= limit) {
			for (int i = 0; i < N; i++) {
				int* nxt = (int*)malloc(sizeof(int)*N);
				for (int k = 0; k < N; k++) nxt[k] = now[k];
				nxt[cnt] = i;
				DLS(N, cnt + 1, limit, nxt, start);
			}
		}
....
```

### 3. BFS

* 너비우선탐색 방법으로, 처음엔 퀸이 하나도 배치가 안 된 상태의 체스판을 queue에 넣는다.
* queue에서 맨 앞에 있는 체스판에 몇 번째 column까지 퀸이 배치되어있는지 확인한다. 

```c
// 현재 체스판 now의 num번째 줄에 퀸을 배치해야함
int* now = (int*)malloc(sizeof(int)*N);
now = Q_pop(&qu);
int num;
for (int i = 0; i < N; i++) {
		if (now[i] == -1) { num = i;  break; }
		if (i == N - 1) num = N;
}
```

* N개의 퀸이 모두 배치되어있으면 완성된 체스판이 답인지 확인한다.
* N개보다 적게 배치되어있으면 다음 차례(num번 column)에 퀸을 0~N-1번 각 row에 하나씩 배치해가며 만들어진 새 체스판을 queue에 넣는다.

```c
if (num == N) {
		if (isok(N, now)) {			// 답을 찾으면 결과 출력 후 함수 끝내기
				result_print("BFS", 1, now, (double)clock() - start);
	....
else {
		for (int i = 0; i < N; i++) {
				int* nxt = (int*)malloc(sizeof(int)*N);		// qu에 새로 넣을 배열 nxt를 만듦
				for (int k = 0; k < N; k++) nxt[k] = now[k];	// now 복사 후 num번 column만 수정
				nxt[num] = i;
				Q_push(&qu, nxt);
	.....
```

* 이를 queue가 빌 때까지 반복한다.

* BFS는 메모리를 많이 차지하기 때문에 마지막에 호출, 배열을 할당하지 못하면 memory limit error가 발생하도록 구현했다.

* BFS에 이용되는 queue를 연결 리스트로 직접 구현하여 사용

* queue를 구성하는 Node 구조체의 int* data가 상위 방법들에서 now배열과 같은 역할을 한다.

  

---
#### isok()

* 각 column에 퀸이 몇 번 row에 위치하는지 나타내는 배열 chk[]을 인자로 받아, 체스판이 안전한지 여부를 판단한다
* chk 배열에 -1이 들어있으면 N개의 퀸이 같은 column에 겹쳐있거나, 퀸의 개수가 N개 미만이라는 뜻이므로 조건에 만족하지 못하는 체스판으로 판단한다.
* 배열에 같은 수가 들어있으면 가로로 공격이 가능하므로 만족하지 못하다고 판단한다.

```c
if (chk[l] == chk[r]) return 0;
```

* 배열의 두 인덱스 값의 차이와 그 인덱스에 해당하는 두 값의 차이가 같으면 대각선으로 공격이 가능하므로 만족하지 못하다고 한단한다.

```c
int d = chk[l] - chk[r];
if (d < 0) d *= -1;
if (d == (r - l)) return 0;
```

#### result_print()

* 탐색 방법의 이름, 답을 찾았는지 여부, 각 column에 퀸이 몇 번 row에 위치하는지 나타내는 배열, 답을 찾을 때까지 걸린 시간을 인자로 받아 형식에 맞게 출력한다

* 각 탐색 함수는 시작할 때 double변수 start에 시간(clock())을 입력하고, 종결할 때 (답을 찾았을 때) start에서 시간(clock())을 뺀 값을 result_print의 인자로 사용한다

  

---


#### 입출력결과

* compile, result4

  ![result_image_1](C:\Users\JaneYi\Assignment\19-2_CSE4007_AI\assignment1\result_image_1.png)

* result3, result7

  ![result_image_2](C:\Users\JaneYi\Assignment\19-2_CSE4007_AI\assignment1\result_image_2.png)