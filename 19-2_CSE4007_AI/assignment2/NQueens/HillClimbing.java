package NQueens;

import java.io.*;
import java.util.*;

public class HillClimbing {

	// ����ؾ��ϴ� �� ���� (string �ڿ� �̾����)
	static String result = ""; 
	
	// main
	public static void main(String args[]) throws IOException {
		int N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // ���� ���� ���
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // ��� ����
		
		int loc[] = new int[N];  // 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
		
		long start = System.currentTimeMillis(); // ���� �ð�
		HC(N, loc, start);
		
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
	
	
	// ��Hill Climbing�� ����� ���� ����� ��� (������� ù ��°�� ã���� ��)
	private static void result_print(int N, int loc[], long time) {
		// loc : 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
		// time : Ž���ϴ� �� �ɸ� �ð�
		
		result += ">Hill Climbing\n";
		
		for(int i = 0; i < N; i++) result += Integer.toString(loc[i]) + " ";
		result += "\n";
		
		result += "Total Elapsed Time : " +  Double.toString(time/1000.0) + "\n\n";
	}
	
	// ���� ��ġ�� N���� Queen �߿� ���� �������� ���ϴ� ��ġ�� �ִ� �ְ� �� ����
	private static int get_state(int N, int chk[]) {
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
	
	// Hill Climbing Search
	private static void HC(int N, int loc[], long start) {
		
		// restart�� Ƚ��
		int restart = 0;
		
		// final �� ã�� ������ �ݺ�
		while(true) {
			// �� column�� �������� �� ��ġ
			Random rand = new Random();
			for(int i=0; i<N; i++) loc[i]=rand.nextInt(N);
			
			// column���� �ϳ��� �Űܺ��鼭 search (local �� ã��)
			while(true) {
				// ���� ��ġ���� loc���� ���� ���� ���ϴ� �� ���� ��������
				int now = get_state(N, loc);
				
				// ������ ���� ��ġ���µ� (�Ű��� ���� ���� �� �� �������� �ϳ��� ���� node)
				// loc[col] = row �� ������ cnt
				ArrayList<Integer> nxt_col = new ArrayList<>();
				ArrayList<Integer> nxt_row = new ArrayList<>();
				int cnt=0;
							
				// i�� column�� �ִ� ���� �Űܺ��� Ž���Ѵ� (loc[i]�� ����)
				for(int i=0; i<N; i++) {
					// ���� ��ġ : save, �ٲ� ��ġ : j
					int save = loc[i]; 
					for(int j=0; j<N; j++) {
						if(j==save) continue; // ���� ���̸� �Ѿ��
									
						// j�� �Ű��� �� ���¿��� ���� ���� ���ϴ� �� ������ ������ ������ ���Ѵ�
						loc[i]=j;
						int nxt = get_state(N, loc);
									
						// �Ű��� ������ ������ �� ������ ���� ��ġ���·� �����ϹǷ� ����Ʈ�� �߰�
						if(nxt>now) {
							cnt++;
							nxt_col.add(i);
							nxt_row.add(j);
						}
						// �ƴϸ� loc ���󺹱�
						else loc[i]=save;
					}
				}
				
				// ���� ���·� ������ �� ������ ������
				if(cnt==0) break;
				// ������ ������ ���µ� �� �������� �ϳ��� ���� node
				else {
					rand = new Random();
					int n = rand.nextInt(cnt);
					loc[nxt_col.get(n)] = nxt_row.get(n);
				}
			}
			
			// ���� loc�� local ��, ���⼭ ���� ���� ���ϴ� �ֵ� ������ N���� final ��
			// final �� ã���� �ݺ� ����, �� ã���� �ٽ� ����
			if (get_state(N, loc) == N) break;
			else restart++;	
			
		}
		result_print(N, loc, System.currentTimeMillis() - start);
		
		
		// restart Ƚ���� ����غ���
		result += "restart : " + Integer.toString(restart);
	}
	
	
}



