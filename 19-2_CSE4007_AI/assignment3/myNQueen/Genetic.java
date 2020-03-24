package myNQueen;


import java.io.*;
import java.util.*;


public class Genetic {
	
	// ����ؾ��ϴ� �� ���� (string �ڿ� �̾����)
	static String result = ""; 
	
	// ��¥ N
	static int N;
	
	static int pp_siz = 5000;		// population size
	static int pr_siz = 500;		// parent size (population size * 0.1)
	static int cross_siz = 4000;	// crossover�� ������� ���� ���� size
	static int mutat_siz = 500;		// mutation���� ������� ���� ���� size
	// ��������(5000) = �θ�(500)+�θ�crossover�Ѱ�(4000)+mutation�Ȱ�(500)

	static int k_siz = 7;		// ��ʸ�Ʈ �����ǿ��� k�� ����
	
	// �ִ� num (�鸸�� �ȿ� �� �� ������ �� �Ǵ� RNA�� �����ϰ� �� kill)
	static int max_num = 1000000;
	
	
	// main
	public static void main(String args[]) throws IOException {
		N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // ���� ���� ���
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // ��� ����

		
		long start = System.currentTimeMillis(); // ���� �ð�
		Gene(start);

		try {
			f.write(result.getBytes()); // ���� f�� ����� ���ڿ� result ����
		}
		catch (Exception e) {
			e.getStackTrace();
		}
		finally {
			f.close(); // ���� �ݱ�
		}
	}
	
	// Ŭ������ �صθ� priority queue ���� ���� �� ����
	static public class RNA implements Comparable<RNA>{
		int[] loc;
		int fit;
		
		// ������
		public RNA(int[] loc) {
			this.loc = loc;
			this.fit = get_fitness(this.loc);
		}
		
		// fitness ������ compare ���� �صθ� priority queue���� �˾Ƽ� ������
		@Override
		public int compareTo(RNA input){
			return this.fit<=input.fit ? 1: -1; // fit�� Ŭ ���� �켱���� ����
		}
		
		@Override
		public boolean equals(Object o) {
			return Arrays.equals(this.loc, ((RNA)o).loc);
		}
	}
	
	// Genetic algorithm�� �̿��� ����� ���� ����� ��� (������� ù ��°�� ã���� ��)
	private static void result_print(int loc[], int num, String from, long time) {
		// loc : 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
		// time : Ž���ϴ� �� �ɸ� �ð�
		// num : �� ��° generation���� ã�� ������
		// from : ��� ������� RNA���� ã�� ������ (parent/crossover/mutation)
		
		result += ">Genetic Algorithm\n";
		
		for(int i = 0; i < N; i++) result += Integer.toString(loc[i]) + " ";
		result += "\n";
		
		result += "(result generation number : " + Integer.toString(num) + ")\n";
		result += "(result RNA from : " + from +")\n";
		
		result += "Total Elapsed Time : " +  Double.toString(time/1000.0) + "\n\n";
	}
	
	// ���� ��ġ�� N���� Queen �߿� ���� �������� ���ϴ� ��ġ�� �ִ� �ְ� �� ����
	// fitness ���� N�� ����� �����ڸ� �����Ѵ� (N�� �Ǹ� ������ ä��) 
	private static int get_fitness(int chk[]) {
		// chk�迭�� 0~N-1 �� ä���� ���� (0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ)
			
		int cnt = 0;
		
		for (int l = 0; l < N; l++) {
			// left�� ���� (�갡 true�� �Ǹ� ���ݰ����� ���̹Ƿ� cnt �ø��� �ʴ´�)
			boolean stat = false;
			
			for (int r = 0; r < N; r++) {
				if (l == r) continue;
				
				// left�� ��ġ�� right�� ��ġ�� ������ ���η� ���� ����
				if (chk[l] == chk[r]) stat = true;
				// left�� ��ġ�� right�� ��ġ�� ���� left~right ���̿� ������ �밢������ ���� ����
				int d = chk[l] - chk[r];
				if (d < 0) d *= -1;
				if (d == (r - l)) stat = true;
			}
			
			if(!stat) cnt++;
		}
		// �������� ���ϴ� �� ���� ����
		return cnt;
	}


	// genetic algorithm
	private static void Gene(long start) {
		
		// RNA�� population�� ������
		ArrayList<RNA> generation = new ArrayList<>();
		
		// �� ó������ ���� ��ġ�� loc���� ���� �Ѵ�
		for(int i=0; i<pp_siz; i++) {
			// 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
			int loc[] = new int[N]; 
			// �� column�� �������� �� ��ġ
			Random rand = new Random();
			for(int j=0; j<N; j++) loc[j]=rand.nextInt(N);
			
			RNA r = new RNA(loc);
			generation.add(r);
		}
		
		// ��ʸ�Ʈ ������
		// generation���� k(=7)���� �̾� -> �� �߿� ���� fit�� �� �ϳ��� parent�� �־�. �� �ݺ� (500)
		// ��������(5000) = �θ�(500)+�θ�crossover�Ѱ�(4000)+mutation�Ȱ�(500)
		
		// fitness ���� N�� �Ǵ� �ְ� ���ε� �� ã�� ������ �ݺ�
		for(int num=1; num<=max_num; num++) {
			
			// �θ� ���� �����
			ArrayList<RNA> par = new ArrayList<>();
			
			for(int i=0; i<pr_siz; i++){
				
				// generation���� �����ϰ� ���� k���� RNA�� priority queue�� �ִ´�				
				PriorityQueue<RNA> pq = new PriorityQueue<>();
				
				for(int j=0; j<k_siz; j++) {
					Random rand = new Random();
					RNA r = generation.get(rand.nextInt(pp_siz));
					
					if(par.contains(r)||pq.contains(r)) j--; // �ߺ��̸� �ٽ� ����
					else pq.offer(r);
				}
				
				// pq���� �� �տ� �ִ� RNA�� �����Ѵ� (fit���� N�� ���� �����Ƿ�)
				RNA frontRNA = pq.poll();
				
				// ���õ� RNA�� parent�� �ִ´� (fit���� N�̸� ���� ã�� �Ŵϱ� ������)
				if(frontRNA.fit == N) {
					result_print(frontRNA.loc, num, "first parent", System.currentTimeMillis() - start);
					return;
				}
				else par.add(frontRNA);
			}
			
			// �� ���� �ʱ�ȭ
			generation.clear();
			// 0~3999 : crossover (4000)
			// 4000~4499 : mutation (500)
			// 4500~5000 : �θ� (500)
			
			
			// �θ� crossover �����
			for(int i=0; i<(int)cross_siz/2; i++){
				// par���� RNA�� �����ϰ� 2�� �̴´�
				Random rand = new Random();
				RNA par1 = par.get(rand.nextInt(pr_siz));
				rand = new Random();
				RNA par2 = par.get(rand.nextInt(pr_siz));
				
				// �ݹ� ��� generation�� �ִ´�
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
				
				// 0~3999 : crossover (4000) (�̶� fit���� N�̸� ���� ã�� �Ŵϱ� ������)
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
			
			
			// mutation �����
			for(int i=0; i<mutat_siz; i++) {
				// crossover�� RNA�� �����ϰ� 1�� �̴´�
				Random rand = new Random();
				RNA cross = generation.get(rand.nextInt(cross_siz));
				
				int[] nxtloc = new int[N];
				for(int j=0; j<N; j++) {
					nxtloc[j] = cross.loc[j];
				}
				
				// ������ġ�� �������� �ٲ㼭 generation�� �ִ´�
				rand = new Random();
				int r_idx = rand.nextInt(N);
				rand = new Random();
				int r_num = rand.nextInt(N);
				
				nxtloc[r_idx] = r_num;
				
				RNA nxt = new RNA(nxtloc);
				
				// 4000~4499 : mutation (500) (�̶� fit���� N�̸� ���� ã�� �Ŵϱ� ������)
				if(nxt.fit == N) {
					result_print(nxt.loc, num, "mutation", System.currentTimeMillis() - start);
					return;
				}
				else generation.add(nxt);
			}
			
			// 4500~5000 : �θ� (500)
			for(int i=0; i<pr_siz; i++){
				generation.add(par.get(i));
			}
			
			
			/*// �� ��° ���밡 ������ ����غ���
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
		
		// ����� ���� max num �Ѿ �߸� �����ƴٴ� �Ŵϱ�
		result += ">Genetic Algorithm\n";
		result += "fail (generation number limit error)\n";
		result += "Total Elapsed Time : " +  Double.toString((System.currentTimeMillis() - start)/1000.0) + "\n\n";
		return;
	}


}


