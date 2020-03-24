package NQueens;

import java.io.*;
import java.util.*;

public class HillClimbing {

	// 출력해야하는 답 저장 (string 뒤에 이어붙임)
	static String result = ""; 
	
	// main
	public static void main(String args[]) throws IOException {
		int N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // 파일 저장 경로
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // 출력 파일
		
		int loc[] = new int[N];  // 0~(N-1)의 Column에 있는 각 Queen의 row 위치
		
		long start = System.currentTimeMillis(); // 시작 시간
		HC(N, loc, start);
		
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
	
	
	// ‘Hill Climbing’ 방법에 대한 결과물 출력 (방법에서 첫 번째로 찾아진 답)
	private static void result_print(int N, int loc[], long time) {
		// loc : 0~(N-1)의 Column에 있는 각 Queen의 row 위치
		// time : 탐색하는 데 걸린 시간
		
		result += ">Hill Climbing\n";
		
		for(int i = 0; i < N; i++) result += Integer.toString(loc[i]) + " ";
		result += "\n";
		
		result += "Total Elapsed Time : " +  Double.toString(time/1000.0) + "\n\n";
	}
	
	// 현재 배치된 N개의 Queen 중에 서로 공격하지 못하는 위치에 있는 애가 몇 갠지
	private static int get_state(int N, int chk[]) {
		// chk배열은 0~N-1 다 채워진 상태 (0~(N-1)의 Column에 있는 각 Queen의 row 위치)
		
		int cnt = 0;
		
		for (int l = 0; l < N; l++) {
			// left의 상태 (얘가 true가 되면 공격가능한 것이므로 cnt 늘리지 않는다)
			boolean stat = false;
			
			for (int r = 0; r < N; r++) {
				if (l == r) continue;
				
				// left의 위치와 right의 위치가 같으면 가로로 공격 가능
				if (chk[l] == chk[r]) stat = true;
				// left의 위치와 right의 위치의 차가 left~right 차이와 같으면 대각선으로 공격 가능
				int d = chk[l] - chk[r];
				if (d < 0) d *= -1;
				if (d == (r - l)) stat = true;
			}
			
			if(!stat) cnt++;
		}
		// 공격하지 못하는 퀸 개수 리턴
		return cnt;
	}
	
	// Hill Climbing Search
	private static void HC(int N, int loc[], long start) {
		
		// restart한 횟수
		int restart = 0;
		
		// final 답 찾을 때까지 반복
		while(true) {
			// 각 column에 랜덤으로 퀸 배치
			Random rand = new Random();
			for(int i=0; i<N; i++) loc[i]=rand.nextInt(N);
			
			// column마다 하나씩 옮겨보면서 search (local 답 찾기)
			while(true) {
				// 현재 배치상태 loc에서 서로 공격 못하는 애 개수 가져오기
				int now = get_state(N, loc);
				
				// 가능한 다음 배치상태들 (옮겨진 후의 여러 개 중 랜덤으로 하나가 다음 node)
				// loc[col] = row 의 개수가 cnt
				ArrayList<Integer> nxt_col = new ArrayList<>();
				ArrayList<Integer> nxt_row = new ArrayList<>();
				int cnt=0;
							
				// i번 column에 있는 퀸을 옮겨보며 탐색한다 (loc[i]를 수정)
				for(int i=0; i<N; i++) {
					// 원래 위치 : save, 바꿀 위치 : j
					int save = loc[i]; 
					for(int j=0; j<N; j++) {
						if(j==save) continue; // 같은 값이면 넘어가기
									
						// j로 옮겼을 때 상태에서 서로 공격 못하는 애 개수를 원래의 개수랑 비교한다
						loc[i]=j;
						int nxt = get_state(N, loc);
									
						// 옮겨진 상태의 개수가 더 많으면 다음 배치상태로 가능하므로 리스트에 추가
						if(nxt>now) {
							cnt++;
							nxt_col.add(i);
							nxt_row.add(j);
						}
						// 아니면 loc 원상복귀
						else loc[i]=save;
					}
				}
				
				// 다음 상태로 가능한 게 없으면 끝내기
				if(cnt==0) break;
				// 있으면 가능한 상태들 중 랜덤으로 하나가 다음 node
				else {
					rand = new Random();
					int n = rand.nextInt(cnt);
					loc[nxt_col.get(n)] = nxt_row.get(n);
				}
			}
			
			// 현재 loc은 local 답, 여기서 서로 공격 못하는 애들 개수가 N개면 final 답
			// final 답 찾으면 반복 종료, 못 찾으면 다시 시작
			if (get_state(N, loc) == N) break;
			else restart++;	
			
		}
		result_print(N, loc, System.currentTimeMillis() - start);
		
		
		// restart 횟수도 출력해보자
		result += "restart : " + Integer.toString(restart);
	}
	
	
}



