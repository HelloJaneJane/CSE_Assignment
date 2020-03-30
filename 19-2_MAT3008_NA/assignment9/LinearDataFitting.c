#include <stdio.h>
#include <stdlib.h>

#define NN 500 // �ִ� observation ��

// X = a1 xi + a2 yi + a3
// Y = a4 xi + a5 yi + a6
float xi[NN], yi[NN], X[NN], Y[NN];

int n; // ������ ����

/*
A =	( s_xi2  s_xiyi  s_xi)
	( s_xiyi s_yi2   s_yi)
	( s_xi   s_yi    n   )

b1 = ( s_xiX )
	 ( s_yiX )
	 ( s_X   )

b2 = ( s_xiY )
	 ( s_yiY )
	 ( s_Y   )
*/

// 3*3 ¥��
float **A, **Ainv;
float s_xi2, s_yi2, s_xiyi, s_xi, s_yi;

// 3*1 ¥��
float **b1, **b2;
float s_xiX, s_yiX, s_X, s_xiY, s_yiY, s_Y;

// �� (a1~a6) 6*1 ¥��
float **ans;



int main() {
	// �Է� �ޱ�
	FILE *fp;
	fp = fopen("fitdata3.dat", "r");
	if (fp == NULL) nrerror("data file open fail\n");

	n = 0;
	while (fscanf(fp, "%f %f %f %f", &xi[n], &yi[n], &X[n], &Y[n]) != EOF) {
		n++;
	}

	// �Էµ� data�� ��� �����
	A = matrix(1, 3, 1, 3);
	Ainv = matrix(1, 3, 1, 3);
	b1 = matrix(1, 3, 1, 1);
	b2 = matrix(1, 3, 1, 1);

	for (int i = 0; i < n; i++) {
		// A
		s_xi2 += xi[i] * xi[i];
		s_yi2 += yi[i] * yi[i];
		s_xiyi += xi[i] * yi[i];
		s_xi += xi[i];
		s_yi += yi[i];
		// b1
		s_xiX += xi[i] * X[i];
		s_yiX += yi[i] * X[i];
		s_X += X[i];
		// b2
		s_xiY += xi[i] * Y[i];
		s_yiY += yi[i] * Y[i];
		s_Y += Y[i];
	}
	// A
	A[1][1] = s_xi2;
	A[1][2] = A[2][1] = s_xiyi;
	A[1][3] = A[3][1] = s_xi;
	A[2][2] = s_yi2;
	A[2][3] = A[3][2] = s_yi;
	A[3][3] = n;
	// b1
	b1[1][1] = s_xiX;
	b1[2][1] = s_yiX;
	b1[3][1] = s_X;
	// b2
	b2[1][1] = s_xiY;
	b2[2][1] = s_yiY;
	b2[3][1] = s_Y;


	// A�� ����� ���ϱ� (assignment5 �� ��� �ڵ� Ȱ��)
	// [A : I] -> [I : A-1] --- A,I�� gaussj �ϸ� A�� I, I�� A-1�� �ȴ�
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			Ainv[i][j] = (i == j ? 1.0 : 0.0);
		}
	}
	gaussj(A, 3, Ainv, 3);


	// �� = A-1 * b
	ans = matrix(1, 6, 1, 1);
	
	for (int i = 1; i <= 3; i++) {
		ans[i][1] = ans[i + 3][1] = 0;
		for (int j = 1; j <= 3; j++) {
			// a1 a2 a3 = A*b1
			ans[i][1] += Ainv[i][j] * b1[j][1];
			// a4 a5 a6 = A*b2
			ans[i + 3][1] += Ainv[i][j] * b2[j][1];
		}
	}
	
	for (int i = 1; i <= 6; i++) {
		printf("a%d : %f\n", i, ans[i][1]);
	}

	fclose(fp);
	free_matrix(A, 1, 3, 1, 3);
	free_matrix(Ainv, 1, 3, 1, 3);
	free_matrix(b1, 1, 3, 1, 1);
	free_matrix(b2, 1, 3, 1, 1);
	free_matrix(ans, 1, 6, 1, 1);
}