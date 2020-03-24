package myNQueen;


import java.io.*;
import java.util.*;


public class Genetic {
	
	// 출력해야하는 답 저장 (string 뒤에 이어붙임)
	static String result = ""; 
	
	// 진짜 N
	static int N;
	
	static int pp_siz = 5000;		// population size
	static int pr_siz = 500;		// parent size (population size * 0.1)
	static int cross_siz = 4000;	// crossover로 만들어진 다음 세대 size
	static int mutat_siz = 500;		// mutation으로 만들어진 다음 세대 size
	// 다음세대(5000) = 부모(500)+부모crossover한거(4000)+mutation된거(500)

	static int k_siz = 7;		// 토너먼트 셀렉션에서 k번 선택
	
	// 최대 num (백만번 안에 답 안 나오면 안 되는 RNA라 생각하고 걍 kill)
	static int max_num = 1000000;
	
	
	// main
	public static void main(String args[]) throws IOException {
		N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // 파일 저장 경로
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // 출력 파일

		
		long start = System.currentTimeMillis(); // 시작 시간
		Gene(start);

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
	
	// 클래스로 해두면 priority queue 에도 넣을 수 있음
	static public class RNA implements Comparable<RNA>{
		int[] loc;
		int fit;
		
		// 생성자
		public RNA(int[] loc) {
			this.loc = loc;
			this.fit = get_fitness(this.loc);
		}
		
		// fitness 값으로 compare 설정 해두면 priority queue에서 알아서 정렬함
		@Override
		public int compareTo(RNA input){
			return this.fit<=input.fit ? 1: -1; // fit이 클 수록 우선순위 높다
		}
		
		@Override
		public boolean equals(Object o) {
			return Arrays.equals(this.loc, ((RNA)o).loc);
		}
	}
	
	// Genetic algorithm을 이용한 방법에 대한 결과물 출력 (방법에서 첫 번째로 찾아진 답)
	private static void result_print(int loc[], int num, String from, long time) {
		// loc : 0~(N-1)의 Column에 있는 각 Queen의 row 위치
		// time : 탐색하는 데 걸린 시간
		// num : 몇 번째 generation에서 찾은 답인지
		// from : 어떻게 만들어진 RNA에서 찾은 답인지 (parent/crossover/mutation)
		
		result += ">Genetic Algorithm\n";
		
		for(int i = 0; i < N; i++) result += Integer.toString(loc[i]) + " ";
		result += "\n";
		
		result += "(result generation number : " + Integer.toString(num) + ")\n";
		result += "(result RNA from : " + from +")\n";
		
		result += "Total Elapsed Time : " +  Double.toString(time/1000.0) + "\n\n";
	}
	
	// 현재 배치된 N개의 Queen 중에 서로 공격하지 못하는 위치에 있는 애가 몇 갠지
	// fitness 값이 N에 가까운 유전자를 선택한다 (N이 되면 답으로 채택) 
	private static int get_fitness(int chk[]) {
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


	// genetic algorithm
	private static void Gene(long start) {
		
		// RNA를 population개 생성함
		ArrayList<RNA> generation = new ArrayList<>();
		
		// 맨 처음에는 랜덤 배치를 loc으로 갖게 한다
		for(int i=0; i<pp_siz; i++) {
			// 0~(N-1)의 Column에 있는 각 Queen의 row 위치
			int loc[] = new int[N]; 
			// 각 column에 랜덤으로 퀸 배치
			Random rand = new Random();
			for(int j=0; j<N; j++) loc[j]=rand.nextInt(N);
			
			RNA r = new RNA(loc);
			generation.add(r);
		}
		
		// 토너먼트 셀렉션
		// generation에서 k(=7)개를 뽑아 -> 그 중에 제일 fit한 거 하나를 parent에 넣어. 를 반복 (500)
		// 다음세대(5000) = 부모(500)+부모crossover한거(4000)+mutation된거(500)
		
		// fitness 값이 N이 되는 애가 답인데 걔 찾을 때까지 반복
		for(int num=1; num<=max_num; num++) {
			
			// 부모 세대 만들기
			ArrayList<RNA> par = new ArrayList<>();
			
			for(int i=0; i<pr_siz; i++){
				
				// generation에서 랜덤하게 뽑은 k개의 RNA를 priority queue에 넣는다				
				PriorityQueue<RNA> pq = new PriorityQueue<>();
				
				for(int j=0; j<k_siz; j++) {
					Random rand = new Random();
					RNA r = generation.get(rand.nextInt(pp_siz));
					
					if(par.contains(r)||pq.contains(r)) j--; // 중복이면 다시 선택
					else pq.offer(r);
				}
				
				// pq에서 맨 앞에 있는 RNA를 선택한다 (fit값이 N에 제일 가까우므로)
				RNA frontRNA = pq.poll();
				
				// 선택된 RNA를 parent에 넣는다 (fit값이 N이면 답을 찾은 거니까 끝내기)
				if(frontRNA.fit == N) {
					result_print(frontRNA.loc, num, "first parent", System.currentTimeMillis() - start);
					return;
				}
				else par.add(frontRNA);
			}
			
			// 앞 세대 초기화
			generation.clear();
			// 0~3999 : crossover (4000)
			// 4000~4499 : mutation (500)
			// 4500~5000 : 부모 (500)
			
			
			// 부모 crossover 만들기
			for(int i=0; i<(int)cross_siz/2; i++){
				// par에서 RNA를 랜덤하게 2개 뽑는다
				Random rand = new Random();
				RNA par1 = par.get(rand.nextInt(pr_siz));
				rand = new Random();
				RNA par2 = par.get(rand.nextInt(pr_siz));
				
				// 반반 섞어서 generation에 넣는다
				int[] nxtloc1 = new int[N];
				int[] nxtloc2 = new int[N];
				for(int j=0; j<N; j++) {
					if(j<(int)N/2) {
						nxtloc1[j] = par1.loc[j];
						nxtloc2[j] = par2.loc[j];
					}
					else {
						nxtloc1[j] = par2.loc[j];
						nxtloc2[j] = par1.loc[j];
					}
				}
				RNA nxt1 = new RNA(nxtloc1);
				RNA nxt2 = new RNA(nxtloc2);
				
				// 0~3999 : crossover (4000) (이때 fit값이 N이면 답을 찾은 거니까 끝내기)
				if(nxt1.fit == N) {
					result_print(nxt1.loc, num, "crossover", System.currentTimeMillis() - start);
					return;
				}
				else generation.add(nxt1);
				if(nxt2.fit == N) {
					result_print(nxt2.loc, num, "crossover", System.currentTimeMillis() - start);
					return;
				}
				else generation.add(nxt2);
			}
			
			
			// mutation 만들기
			for(int i=0; i<mutat_siz; i++) {
				// crossover된 RNA를 랜덤하게 1개 뽑는다
				Random rand = new Random();
				RNA cross = generation.get(rand.nextInt(cross_siz));
				
				int[] nxtloc = new int[N];
				for(int j=0; j<N; j++) {
					nxtloc[j] = cross.loc[j];
				}
				
				// 랜덤위치를 랜덤수로 바꿔서 generation에 넣는다
				rand = new Random();
				int r_idx = rand.nextInt(N);
				rand = new Random();
				int r_num = rand.nextInt(N);
				
				nxtloc[r_idx] = r_num;
				
				RNA nxt = new RNA(nxtloc);
				
				// 4000~4499 : mutation (500) (이때 fit값이 N이면 답을 찾은 거니까 끝내기)
				if(nxt.fit == N) {
					result_print(nxt.loc, num, "mutation", System.currentTimeMillis() - start);
					return;
				}
				else generation.add(nxt);
			}
			
			// 4500~5000 : 부모 (500)
			for(int i=0; i<pr_siz; i++){
				generation.add(par.get(i));
			}
			
			
			/*// 몇 번째 세대가 뭔지도 출력해보자
			result += "generation num : " + Integer.toString(num)+"\n";
			
			result += "\tcrossover\n";
			for(int i=0; i<cross_siz; i++) {
				for(int j=0; j<N; j++) result += Integer.toString(generation.get(i).loc[j]);
				result += "\n";
			}
			result += "\tmutation\n";
			for(int i=0; i<mutat_siz; i++) {
				for(int j=0; j<N; j++) result += Integer.toString(generation.get(cross_siz+i).loc[j]);
				result += "\n";
			}
			result += "\tparent\n";
			for(int i=0; i<pr_siz; i++) {
				for(int j=0; j<N; j++) result += Integer.toString(generation.get(cross_siz+mutat_siz+i).loc[j]);
				result += "\n";
			}*/
			
		}
		
		// 여기로 오면 max num 넘어서 잘못 유전됐다는 거니까
		result += ">Genetic Algorithm\n";
		result += "fail (generation number limit error)\n";
		result += "Total Elapsed Time : " +  Double.toString((System.currentTimeMillis() - start)/1000.0) + "\n\n";
		return;
	}


}


