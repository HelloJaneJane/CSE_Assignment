#include <stdio.h>
#include <stdlib.h>

#define NN 20 // 최대 N값

float **A, **b;
int n;

float** x_GJE;
float* x_LUD;
float* x_SVD;

void GJE() {
	// [A : b] -> [I : x]
	// A,b를 gaussj 하면 A는 I, b는 x가 된다

	float **AtoI, **btox;
	AtoI = matrix(1, NN, 1, NN);
	btox = matrix(1, NN, 1, 1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			AtoI[i][j] = A[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		btox[i][1] = b[i][1];
	}

	gaussj(AtoI, n, btox, 1);

	printf("\n  [solution] x :\n");
	for (int i = 1; i <= n; i++) {
		printf("%20.10f ", btox[i][1]);
	}
	printf("\n");


	// 전역으로 저장 (ITIP 때 필요)
	x_GJE = matrix(1, NN, 1, 1);
	for (int i = 1; i <= n; i++) {
		x_GJE[i][1] = btox[i][1];
	}

	free_matrix(AtoI, 1, NN, 1, NN);
	free_matrix(btox, 1, NN, 1, 1);
}

void LUD() {
	// A = L * U
	// A를 ludcmp 하면 A=LU를 만족하는 L, U를 합친 게 된다.
	float **AtoLU; AtoLU = matrix(1, NN, 1, NN);
	int *indx; indx = ivector(1, NN);
	float d;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			AtoLU[i][j] = A[i][j];
		}
	}

	ludcmp(AtoLU, n, indx, &d);

	// A에서 L이랑 U 분리하기
	float **UofA, **LofA;
	UofA = matrix(1, NN, 1, NN);
	LofA = matrix(1, NN, 1, NN);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i < j) {
				UofA[i][j] = AtoLU[i][j];
				LofA[i][j] = 0.0;
			}
			else if (i > j) {
				UofA[i][j] = 0.0;
				LofA[i][j] = AtoLU[i][j];
			}
			else {
				UofA[i][j] = AtoLU[i][j];
				LofA[i][j] = 1.0;
			}
		}
	}
	printf("\n  L :\n");
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%20.10f ", LofA[i][j]);
		}
		printf("\n");
	}
	printf("\n  U :\n");
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%20.10f ", UofA[i][j]);
		}
		printf("\n");
	}

	// 답 찾기
	float *btox;
	btox = vector(1, NN);
	for (int i = 1; i <= n; i++) btox[i] = b[i][1];
	
	lubksb(AtoLU, n, indx, btox);

	printf("\n  [solution] x :\n");
	for (int i = 1; i <= n; i++)
		printf("%20.10f ", btox[i]);
	printf("\n");

	// 전역으로 저장 (ITIP 때 필요)
	x_LUD = vector(1, NN);
	for (int i = 1; i <= n; i++) {
		x_LUD[i] = btox[i];
	}

	free_matrix(AtoLU, 1, NN, 1, NN);
	free_matrix(UofA, 1, NN, 1, NN);
	free_matrix(LofA, 1, NN, 1, NN);
	free_ivector(indx, 1, NN);
	free_vector(btox, 1, NN);
}

void SVD() {
	// A = U * W * V^T
	// A^-1 = V * [diag(1/wj)] * U^T
	// x = V * [diag(1/wj)] * U^T * b

	// A를 svdcmp 하면 A=UWV^T를 만족하는 U가 된다
	float **AtoU, **VofA;
	AtoU = matrix(1, NN, 1, NN);
	VofA = matrix(1, NN, 1, NN);
	float *WofA; WofA = vector(1, NN);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			AtoU[i][j] = A[i][j];
		}
	}

	svdcmp(AtoU, n, n, WofA, VofA);
	
	printf("\n  U :\n");
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%20.10f ", AtoU[i][j]);
		}
		printf("\n");
	}
	printf("\n  W :\n");
	for (int i = 1; i <= n; i++) {
		printf("%20.10f ", WofA[i]);
	}
	printf("\n");
	printf("\n  V :\n");
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%20.10f ", VofA[i][j]);
		}
		printf("\n");
	}
	

	// 답 찾기
	printf("\n  [solution] x :\n");
	float *copyb, *solx;
	copyb = vector(1, NN);
	solx = vector(1, NN);

	for (int i = 1; i <= n; i++) copyb[i] = b[i][1];
	svbksb(AtoU, WofA, VofA, n, n, copyb, solx);
	for (int i = 1; i <= n; i++) printf("%20.10f ", solx[i]);
	printf("\n");


	// 전역으로 저장 (ITIP 때 필요)
	x_SVD = vector(1, NN);
	for (int i = 1; i <= n; i++) {
		x_SVD[i] = solx[i];
	}

	free_matrix(AtoU, 1, NN, 1, NN);
	free_matrix(VofA, 1, NN, 1, NN);
	free_vector(WofA, 1, NN);
	free_vector(copyb, 1, NN);
	free_vector(solx, 1, NN);
}

void ITIP() {
	// A를 ludcmp 하면 A=LU를 만족하는 L, U를 합친 게 된다.
	float **AtoLU; AtoLU = matrix(1, NN, 1, NN);
	int *indx; indx = ivector(1, NN);
	float d;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			AtoLU[i][j] = A[i][j];
		}
	}

	float *copyb; copyb = vector(1, NN);
	for (int i = 1; i <= n; i++) copyb[i] = b[i][1];

	ludcmp(AtoLU, n, indx, &d);

	float* x;
	x = vector(1, NN);

	// 1. GJE
	for (int i = 1; i <= n; i++) x[i] = x_GJE[i][1];
	mprove(A, AtoLU, n, indx, copyb, x);

	printf("\n  GJE improvement solution :\n");
	for (int i = 1; i <= n; i++) printf("%20.10f ", x[i]);
	printf("\n");

	// 2. LUD
	for (int i = 1; i <= n; i++) x[i] = x_LUD[i];
	mprove(A, AtoLU, n, indx, copyb, x);

	printf("\n  LUD improvement solution :\n");
	for (int i = 1; i <= n; i++) printf("%20.10f ", x[i]);
	printf("\n");

	// 3. SVD
	for (int i = 1; i <= n; i++) x[i] = x_SVD[i];
	mprove(A, AtoLU, n, indx, copyb, x);

	printf("\n  SVD improvement solution :\n");
	for (int i = 1; i <= n; i++) printf("%20.10f ", x[i]);
	printf("\n");


	free_matrix(AtoLU, 1, NN, 1, NN);
	free_ivector(indx, 1, NN);
	free_vector(x, 1, NN);
	free_vector(copyb, 1, NN);
}

void get_inv() {
	// [A : I] -> [I : A-1]
	// A,I를 gaussj 하면 A는 I, I는 A-1가 된다

	float **AtoI, **ItoAi;
	AtoI = matrix(1, NN, 1, NN);
	ItoAi = matrix(1, NN, 1, NN);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			AtoI[i][j] = A[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			ItoAi[i][j] = (i == j ? 1.0 : 0.0);
		}
	}

	gaussj(AtoI, n, ItoAi, n);

	printf("\n  [solution] inverse of A :\n");
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			printf("%20.10f ", ItoAi[i][j]);
		}
		printf("\n");
	}

	free_matrix(AtoI, 1, NN, 1, NN);
	free_matrix(ItoAi, 1, NN, 1, 1);
}

void get_det() {
	float **saveA;
	saveA = matrix(1, NN, 1, NN);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			saveA[i][j] = A[i][j];
		}
	}

	// A를 upper triangular로 바꾼다 (그래야 determinant 구하기 쉬우니까)
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i >= j) continue;
			if (saveA[i][i] == 0) continue;
			float tmp = (float) saveA[j][i] / saveA[i][i];
			for (int k = 1; k <= n; k++) {
				saveA[j][k] -= saveA[i][k] * tmp;
			}
		}
	}

	// 주 대각선 성분 곱하면 답
	float sol = 1.0;
	for (int i = 1; i <= n; i++) {
		sol *= saveA[i][i];
	}

	printf("\n  [solution] determinant of A : %20.10f\n", sol);

	free_matrix(saveA, 1, NN, 1, NN);
}

int main() {
	// [A (N*N)] * [x (N*M)] = [b (N*M)]

	// 할당시켜서 행렬 만들어놓기
	
	A = matrix(1, NN, 1, NN); // N rows, N cols
	b = matrix(1, NN, 1, 1); // N rows, 1 col

	// 입력 받기
	FILE *fp;

	fp = fopen("lineq2.dat", "r");
	if (fp == NULL) nrerror("data file open fail\n");

	fscanf(fp, "%d %d ", &n, &n); // ???????????

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			fscanf(fp, "%f ", &A[i][j]);
		}
	}

	for (int i = 1; i <= n; i++) {
		fscanf(fp, "%f ", &b[i][1]);
	}

	printf("\n\n----- <1> Gauss-Jordan Elimination ---------------\n");
	GJE();

	printf("\n\n----- <2> LU Decomposition ---------------\n");
	LUD();

	printf("\n\n----- <3> Singular Value Decomposition ---------------\n");
	SVD();

	printf("\n\n----- Iterative Improvement ---------------\n");
	ITIP();

	printf("\n\n----- get inverse of A ---------------\n");
	get_inv();

	printf("\n\n----- get determinant of A ---------------\n");
	get_det();

	fclose(fp);
	free_matrix(A, 1, NN, 1, NN);
	free_matrix(b, 1, NN, 1, 1);
	free_matrix(x_GJE, 1, NN, 1, 1);
	free_vector(x_LUD, 1, NN);
	free_vector(x_SVD, 1, NN);
}