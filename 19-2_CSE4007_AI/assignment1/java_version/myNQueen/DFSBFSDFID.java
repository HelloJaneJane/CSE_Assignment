package myNQueen;

import java.io.*;
import java.util.*;

public class DFSBFSDFID {
	
	// ����ؾ��ϴ� �� ���� (string �ڿ� �̾����)
	static String result = ""; 
	
	// 0. main
	public static void main(String args[]) throws IOException {
		int N = Integer.parseInt(args[0]);
		
		String path = args[1] + "/result" + args[0] + ".txt"; // ���� ���� ���
		BufferedOutputStream f = new BufferedOutputStream(new FileOutputStream(path)); // ��� ����
		
		int loc[] = new int[N];  // 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
		
		// DFS
		long start = System.currentTimeMillis(); // ���� �ð�
		DFS(N, 0, loc, start);
		
		// BFS
		start = System.currentTimeMillis();
		BFS(N, loc, start);
		
		// DFID
		start = System.currentTimeMillis();
		DFID(N, loc, start);
		
		
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
	
	
	
	// ��DFS, BFS, DFID Search�� ����� ���� ����� ��� (�� ������� ù ��°�� ã���� ��)
	private static void result_print(int N, String name, boolean solv, int loc[], long time) {
		// name : Ž�� ��� �̸�
		// solv : ���� �ִ��� ������
		// loc : 0~(N-1)�� Column�� �ִ� �� Queen�� row ��ġ
		// time : Ž���ϴ� �� �ɸ� �ð�
		
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
	
	// ���� ��ġ�� N���� Queen���� ���� �������� ���ϴ� ��ġ�� �ִ��� �Ǵ�
	private static boolean isok(int N, int chk[]) {
		// chk�迭�� 0~N-1 �� ä���� ���¿��� �� -> -1�� ��������� 0 ����
		for (int i = 0; i < N; i++) {
			if (chk[i] == -1) return false;
		}
		// ���� �����ϸ� 0, �ƴϸ� 1 return
		for (int l = 0; l < N - 1; l++) {
			for (int r = l + 1; r < N; r++) {
				// left�� ��ġ�� right�� ��ġ�� ������ ���η� ���� ����
				if (chk[l] == chk[r]) return false;
				// left�� ��ġ�� right�� ��ġ�� ���� left~right ���̿� ������ �밢������ ���� ����
				int d = chk[l] - chk[r];
				if (d < 0) d *= -1;
				if (d == (r - l)) return false;
			}
		}
		// ������� �� ��������� �����ϴ� ���̹Ƿ� 1�� return�Ѵ�
		return true;
	}
	
	
	
	// 1. DFS
	
	// ù ��° ���� �������� ret�� true�� �ٲ۴�
	static boolean DFSret = false;
	
	// ��ġ�迭�� ��� N-1�̸� �� ������ ����̹Ƿ� 1�� ����
	private static boolean islast(int N, int chk[]) {
		for (int i = 0; i < N; i++) {
			if (chk[i] != N - 1) return false;
		}
		return true;
	}
	
	// Depth First Search : ���� �켱 Ž��
	private static void DFS(int N, int cnt, int now[], long start) {
		
		// ���� ���� ���Ŀ� ȣ��Ǵ� �Լ��� �������� ����
		if(DFSret) return;
		
		// DFS�� ���� ó�� ȣ��� �� �ʱ�ȭ 
		if (cnt == 0) {
			for(int i = 0; i < N; i++) now[i] = -1;
		}
		
		// ������� ���� cnt�� ��ġ�� -> cnt�� column�� ��ġ�� ����
		
		// ��ġ�� �� ������ N�̸� isok �Ǵ�
		if (cnt == N) {
			// ���� ã���� ��� ��� �� �Լ� ������
			if (isok(N, now)) {
				result_print(N, "DFS", true, now, System.currentTimeMillis() - start);
				DFSret = true;
				return;
			}
			// �� ã�Ҵµ� ���� �����ִ� �� �������̸� �� ����
			else {
				if (islast(N, now)) {
					result_print(N, "DFS", false, now, System.currentTimeMillis() - start);
				}
			}
		}
		// cnt�� column�� ���� ��ġ (0~N-1�� row)
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
	
	// Breadth First Search : �ʺ� �켱 Ž��
	private static void BFS(int N, int first[], long start) {
		
		Queue<int[]> qu = new LinkedList<int[]>();
		
		// �ʱ�ȭ ��Ų ù ü������ qu�� push
		for (int i = 0; i < N; i++) first[i] = -1;
		qu.offer(first);
		
		// qu�� �� ������ �ݺ�
		while (!qu.isEmpty()) {
			// ���� ü���� now�� num��° �ٿ� ���� ��ġ�ؾ���
			
			// qu�� pop�ؼ� now�� ����, num�� ã�Ƴ���
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
			
			// N-1��° �ٱ��� ���� ��� ��ġ�Ǿ����� isok �Ǵ� 
			if (num == N) {
				// ���� ã���� ��� ��� �� �Լ� ������
				if (isok(N, now)) {
					result_print(N, "BFS", true, now, System.currentTimeMillis() - start);
					return;
				}
				// �� ã���� ��� �����ϱ�
				else {
					continue;
				}
			}
			
			// ���� �� ��ġ�Ǿ����� num�� column�� ���� ��ġ�� (0��row ~ N-1��row) �� qu�� �ְ� �ݺ�
			else {
				for (int i = 0; i < N; i++) {
					// qu�� ���� ���� �迭 nxt�� ����
					int nxt[] = new int[N];
					// now ���� �� num�� column�� ����
					for (int k = 0; k < N; k++) nxt[k] = now[k];
					nxt[num] = i;
					
					qu.offer(nxt);
				}
			}
		}
		// qu�� �� ������ return���� �� ������ ���� ����
		result_print(N, "BFS", false, first, System.currentTimeMillis() - start);
		return;
	}



	// 3. DFID
	
	// ù ��° ���� �������� ret�� 1�� �ٲ۴�
	static boolean DFIDret = false;
	
	// DFS - Iterative Deepening Search : ������ ���� ���� Ž��
	private static void DFID(int N, int now[], long start) {
		
		// depth : Ʈ�� ����(��Ʈ����=0 ���� ���) && ���� ��ġ�� �� ����
		for (int depth = 0; depth <= N; depth++) {
			// depth(limit) ���� ������ ������ �ʱ�ȭ
			for (int i = 0; i < N; i++) now[i] = -1;
			
			// DLS ��� ���� ã���� return
			DLS(N, 0, depth, now, start);
		}
		
		// depth�� ������ Ž���� ������ return���� �� ������ ���� ����
		if (!DFIDret) result_print(N, "DFID", false, now, System.currentTimeMillis() - start);
	}
	
	// Depth Limited Search : ���� ���� Ž��
	private static void DLS(int N, int cnt, int limit, int now[], long start) {
		// ���� ���� ���Ŀ� ȣ��Ǵ� �Լ��� �������� ����
		if (DFIDret) return;
		
		// cnt : ������� ��ġ�� �� ����
		// limit : ���� ���� (cnt�� 0���� limit������ ����)
		
		// ���̰� 0�̸� �ٷ� return
		if (limit == 0) return;
		
		// ���̰� N�̸� ������ Ȯ���ϱ�
		else if (cnt == N && limit == N) {
			// ���� ã���� ��� ��� �� �Լ� ������
			if (isok(N, now)) {
				result_print(N, "DFID", true, now, System.currentTimeMillis() - start);
				DFIDret = true;
				return;
			}
		}
		
		// �� �ܿ��� dfs������
		else {
			// cnt�� column�� ��ġ�� ���� (0~N-1�� row)
			// ���� cnt�� limit���� �۰ų� ���� ���� ��� �� ����
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