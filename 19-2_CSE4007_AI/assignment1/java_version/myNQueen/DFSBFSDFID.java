package myNQueen;

import java.io.*;
import java.util.*;

public class DFSBFSDFID {
	
	// 출력해야하는 답 저장 (string 뒤에 이어붙임)
	static String result = ""; 
	
	// 0. main
	public static void main(String args[]) throws IOException {
		int N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // 파일 저장 경로
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // 출력 파일
		
		int loc[] = new int[N];  // 0~(N-1)의 Column에 있는 각 Queen의 row 위치
		
		// DFS
		long start = System.currentTimeMillis(); // 시작 시간
		DFS(N, 0, loc, start);
		
		// BFS
		start = System.currentTimeMillis();
		BFS(N, loc, start);
		
		// DFID
		start = System.currentTimeMillis();
		DFID(N, loc, start);
		
		
		try {
			f.write(result.getBytes()); // 파일 f에 결과물 문자열 result 쓰기
		}
		catch (Exception e) {
			e.getStackTrace();
		}
		finally {
			f.close(); // 파일 닫기
		}
	}
	
	
	
	// ‘DFS, BFS, DFID Search’ 방법에 대한 결과물 출력 (각 방법에서 첫 번째로 찾아진 답)
	private static void result_print(int N, String name, boolean solv, int loc[], long time) {
		// name : 탐색 방법 이름
		// solv : 답이 있는지 없는지
		// loc : 0~(N-1)의 Column에 있는 각 Queen의 row 위치
		// time : 탐색하는 데 걸린 시간
		
		result += ">" + name + "\n";
		
		if(solv) {
			result += "Location : ";
			for(int i = 0; i < N; i++) result += Integer.toString(loc[i]) + " ";
			result += "\n";
		}
		else {
			result += "No solution\n";
		}
		
		result += "Time : " +  Double.toString(time/1000.0) + "\n\n";
	}
	
	// 현재 배치된 N개의 Queen들이 서로 공격하지 못하는 위치에 있는지 판단
	private static boolean isok(int N, int chk[]) {
		// chk배열은 0~N-1 다 채워진 상태여야 함 -> -1이 들어있으면 0 리턴
		for (int i = 0; i < N; i++) {
			if (chk[i] == -1) return false;
		}
		// 공격 가능하면 0, 아니면 1 return
		for (int l = 0; l < N - 1; l++) {
			for (int r = l + 1; r < N; r++) {
				// left의 위치와 right의 위치가 같으면 가로로 공격 가능
				if (chk[l] == chk[r]) return false;
				// left의 위치와 right의 위치의 차가 left~right 차이와 같으면 대각선으로 공격 가능
				int d = chk[l] - chk[r];
				if (d < 0) d *= -1;
				if (d == (r - l)) return false;
			}
		}
		// 여기까지 다 통과했으면 만족하는 답이므로 1을 return한다
		return true;
	}
	
	
	
	// 1. DFS
	
	// 첫 번째 답이 구해지면 ret을 true로 바꾼다
	static boolean DFSret = false;
	
	// 위치배열이 모두 N-1이면 맨 마지막 노드이므로 1을 리턴
	private static boolean islast(int N, int chk[]) {
		for (int i = 0; i < N; i++) {
			if (chk[i] != N - 1) return false;
		}
		return true;
	}
	
	// Depth First Search : 깊이 우선 탐색
	private static void DFS(int N, int cnt, int now[], long start) {
		
		// 답이 나온 이후에 호출되는 함수는 실행하지 않음
		if(DFSret) return;
		
		// DFS가 가장 처음 호출될 때 초기화 
		if (cnt == 0) {
			for(int i = 0; i < N; i++) now[i] = -1;
		}
		
		// 현재까지 퀸을 cnt개 배치함 -> cnt번 column에 배치할 차례
		
		// 배치된 퀸 개수가 N이면 isok 판단
		if (cnt == N) {
			// 답을 찾으면 결과 출력 후 함수 끝내기
			if (isok(N, now)) {
				result_print(N, "DFS", true, now, System.currentTimeMillis() - start);
				DFSret = true;
				return;
			}
			// 못 찾았는데 지금 보고있는 게 마지막이면 답 없음
			else {
				if (islast(N, now)) {
					result_print(N, "DFS", false, now, System.currentTimeMillis() - start);
				}
			}
		}
		// cnt번 column에 퀸을 배치 (0~N-1번 row)
		else {
			for (int i = 0; i < N; i++) {
				int nxt[] = new int[N];
				for (int k = 0; k < N; k++) nxt[k] = now[k];
				nxt[cnt] = i;
				DFS(N, cnt + 1, nxt, start);
			}
		}
	}
	
	
	
	// 2. BFS
	
	// Breadth First Search : 너비 우선 탐색
	private static void BFS(int N, int first[], long start) {
		
		Queue<int[]> qu = new LinkedList<int[]>();
		
		// 초기화 시킨 첫 체스판을 qu에 push
		for (int i = 0; i < N; i++) first[i] = -1;
		qu.offer(first);
		
		// qu가 빌 때까지 반복
		while (!qu.isEmpty()) {
			// 현재 체스판 now의 num번째 줄에 퀸을 배치해야함
			
			// qu를 pop해서 now에 저장, num값 찾아내기
			int now[] = new int[N];
			System.arraycopy(qu.peek(), 0, now, 0, qu.peek().length);
			qu.poll();
			int num = 0;
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
					result_print(N, "BFS", true, now, System.currentTimeMillis() - start);
					return;
				}
				// 못 찾으면 계속 진행하기
				else {
					continue;
				}
			}
			
			// 퀸이 덜 배치되었으면 num번 column에 퀸을 배치한 (0번row ~ N-1번row) 후 qu에 넣고 반복
			else {
				for (int i = 0; i < N; i++) {
					// qu에 새로 넣을 배열 nxt를 만듦
					int nxt[] = new int[N];
					// now 복사 후 num번 column만 수정
					for (int k = 0; k < N; k++) nxt[k] = now[k];
					nxt[num] = i;
					
					qu.offer(nxt);
				}
			}
		}
		// qu가 빌 때까지 return하지 못 했으면 답이 없음
		result_print(N, "BFS", false, first, System.currentTimeMillis() - start);
		return;
	}



	// 3. DFID
	
	// 첫 번째 답이 구해지면 ret을 1로 바꾼다
	static boolean DFIDret = false;
	
	// DFS - Iterative Deepening Search : 점차적 깊이 제한 탐색
	private static void DFID(int N, int now[], long start) {
		
		// depth : 트리 깊이(루트깊이=0 으로 계산) && 현재 배치된 퀸 개수
		for (int depth = 0; depth <= N; depth++) {
			// depth(limit) 새로 시작할 때마다 초기화
			for (int i = 0; i < N; i++) now[i] = -1;
			
			// DLS 결과 값을 찾으면 return
			DLS(N, 0, depth, now, start);
		}
		
		// depth를 끝까지 탐색할 때까지 return하지 못 했으면 답이 없음
		if (!DFIDret) result_print(N, "DFID", false, now, System.currentTimeMillis() - start);
	}
	
	// Depth Limited Search : 깊이 제한 탐색
	private static void DLS(int N, int cnt, int limit, int now[], long start) {
		// 답이 나온 이후에 호출되는 함수는 실행하지 않음
		if (DFIDret) return;
		
		// cnt : 현재까지 배치한 퀸 개수
		// limit : 제한 깊이 (cnt는 0부터 limit까지만 가능)
		
		// 깊이가 0이면 바로 return
		if (limit == 0) return;
		
		// 깊이가 N이면 답인지 확인하기
		else if (cnt == N && limit == N) {
			// 답을 찾으면 결과 출력 후 함수 끝내기
			if (isok(N, now)) {
				result_print(N, "DFID", true, now, System.currentTimeMillis() - start);
				DFIDret = true;
				return;
			}
		}
		
		// 그 외에는 dfs돌리기
		else {
			// cnt번 column에 배치할 차례 (0~N-1번 row)
			// 다음 cnt가 limit보다 작거나 같을 때만 재귀 더 진행
			if (cnt + 1 <= limit) {
				for (int i = 0; i < N; i++) {
					int nxt[] = new int[N];
					for (int k = 0; k < N; k++) nxt[k] = now[k];
					nxt[cnt] = i;
					DLS(N, cnt + 1, limit, nxt, start);
				}
			}
		}
	}
}