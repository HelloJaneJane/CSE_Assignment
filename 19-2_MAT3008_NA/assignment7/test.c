#include <stdio.h>
#define NRANSI
#include "nr.h"
#include "nrutil.h"

#define N 11

#define m 0.0 // 평균
#define s 1.0 // 표준편차

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)


int main(void) {

	float randnum[N];

	long idum = IM;
	// gaussian distribution N(m, s^2)
	for (int i = 0; i < N; i++) {
		randnum[i] = s * gasdev(&idum) + m;
	}

	float A[N][N];

	// randnum에 있는 수 11개로 symmetric 행렬 A 만들기
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i <= j) A[i][j] = randnum[j];
			else A[i][j] = randnum[i];
		}
	}

	// A 출력
	printf("A:\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%11.6f", A[i][j]);
		}
		printf("\n");
	}

	int nrot;
	float *d, *r, **v, **e;
	d = vector(1, N);
	r = vector(1, N);
	v = matrix(1, N, 1, N);
	e = convert_matrix(&A[0][0], 1, N, 1, N);

	// 계산
	jacobi(e, N, d, v, &nrot);
	eigsrt(d, v, N);

	// 출력
	printf("eigenvalues:\n");
	for (int i = 1; i <= N; i++) {
		printf("%11.6f", d[i]);
	}
	printf("\neigenvectors:\n");
	for (int i = 1; i <= N; i++) {
		printf("[%2d ] \n", i);
		for (int j = 1; j <= N; j++) {
			printf("%11.6f", v[j][i]);
		}
		printf("\n");
	}

	
	free_convert_matrix(e, 1, N, 1, N);
	free_matrix(v, 1, N, 1, N);
	free_vector(r, 1, N);
	free_vector(d, 1, N);
	return 0;
}