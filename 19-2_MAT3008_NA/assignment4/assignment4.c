#include <math.h>
#include <stdio.h>

#define MAXIT 50 // 최대 iteration 횟수 제한을 50으로 통일

// 치환
#define T (double)sqrt(2000 - 0.01*R*R)

// 주어진 함수
double F(double R) {
	return pow(exp(1.0), -0.005*R)*cos(0.05*T) - 0.01;
}
// 주어진 함수의 미분
double F_(double R) {
	return -0.005*pow(exp(1.0), -0.005*R)*(cos(0.05*T) - 0.1*R*sin(0.05*T) / T);
}

// newton raphson 방법 사용 시 필요한 포인터 함수
void Fd(double R, double* fn, double* df) {
	*fn = F(R);
	*df = F_(R);
}

// 에러 메세지 함수
void nrerror(char error_text[]) {
	printf("NR ERROR : %s\n", error_text);
}

// 프린트 함수
void print(char name[], double xacc, double root, int iter) {
	printf("[%s method]\n", name);
	printf("10^(%d)\t %2d iterations -> root : %.10lf\n\n", (int)log10(xacc), iter, root);
}

// bisection
double rtbis(double(*func)(double), double x1, double x2, double xacc) {
	int j;
	double dx, f, fmid, xmid, rtb;

	f = (*func)(x1);
	fmid = (*func)(x2);
	if (f*fmid >= 0.0) nrerror("Root must be bracketed for bisection in rtbis");
	rtb = f < 0.0 ? (dx = x2 - x1, x1) : (dx = x1 - x2, x2);
	for (j = 1; j <= MAXIT; j++) {
		fmid = (*func)(xmid = rtb + (dx *= 0.5));
		if (fmid <= 0.0) rtb = xmid;
		if (fabs(dx) < xacc || fmid == 0.0) {
			print("Bisection", xacc, rtb, j);
			return rtb;
		}
	}
	nrerror("Too many bisections in rtbis");
	return 0.0;
} 

// linear interpolation
double rtflsp(double(*func)(double), double x1, double x2, double xacc) {
	int j;
	double fl, fh, xl, xh, swap, dx, del, f, rtf;

	fl = (*func)(x1);
	fh = (*func)(x2);
	if (fl*fh > 0.0) nrerror("Root must be bracketed in rtflsp");
	if (fl < 0.0) {
		xl = x1;
		xh = x2;
	}
	else {
		xl = x2;
		xh = x1;
		swap = fl;
		fl = fh;
		fh = swap;
	}
	dx = xh - xl;
	for (j = 1; j <= MAXIT; j++) {
		rtf = xl + dx * fl / (fl - fh);
		f = (*func)(rtf);
		if (f < 0.0) {
			del = xl - rtf;
			xl = rtf;
			fl = f;
		}
		else {
			del = xh - rtf;
			xh = rtf;
			fh = f;
		}
		dx = xh - xl;
		if (fabs(del) < xacc || f == 0.0) {
			print("Linear Interpolation", xacc, rtf, j);
			return rtf;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtflsp");
	return 0.0;
}

// secant
double rtsec(double(*func)(double), double x1, double x2, double xacc){
	int j;
	double fl, f, dx, swap, xl, rts;

	fl = (*func)(x1);
	f = (*func)(x2);
	if (fabs(fl) < fabs(f)) {
		rts = x1;
		xl = x2;
		swap = fl;
		fl = f;
		f = swap;
	}
	else {
		xl = x1;
		rts = x2;
	}
	for (j = 1; j <= MAXIT; j++) {
		dx = (xl - rts)*f / (f - fl);
		xl = rts;
		fl = f;
		rts += dx;
		f = (*func)(rts);
		if (fabs(dx) < xacc || f == 0.0) {
			print("Secant", xacc, rts, j);
			return rts;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtsec");
	return 0.0;
}

// newton raphson
double rtnewt(void(*funcd)(double, double *, double *), double x1, double x2, double xacc) {
	int j;
	double df, dx, f, rtn;

	rtn = (double)0.5*(x1 + x2);
	for (j = 1; j <= MAXIT; j++) {
		(*funcd)(rtn, &f, &df);
		dx = f / df;
		rtn -= dx;
		if ((x1 - rtn)*(rtn - x2) < 0.0)
			nrerror("Jumped out of brackets in rtnewt");
		if (fabs(dx) < xacc) {
			print("Newton Raphson", xacc, rtn, j);
			return rtn;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtnewt");
	return 0.0;
}

// newton with bracketing
double rtsafe(void(*funcd)(double, double *, double *), double x1, double x2, double xacc) {
	int j;
	double df, dx, dxold, f, fh, fl;
	double temp, xh, xl, rts;

	(*funcd)(x1, &fl, &df);
	(*funcd)(x2, &fh, &df);
	if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
		nrerror("Root must be bracketed in rtsafe");
	if (fl == 0.0) return x1;
	if (fh == 0.0) return x2;
	if (fl < 0.0) {
		xl = x1;
		xh = x2;
	}
	else {
		xh = x1;
		xl = x2;
	}
	rts = (double)0.5*(x1 + x2);
	dxold = (double)fabs(x2 - x1);
	dx = dxold;
	(*funcd)(rts, &f, &df);
	for (j = 1; j <= MAXIT; j++) {
		if ((((rts - xh)*df - f)*((rts - xl)*df - f) > 0.0)
			|| (fabs(2.0*f) > fabs(dxold*df))) {
			dxold = dx;
			dx = (double)0.5*(xh - xl);
			rts = xl + dx;
			if (xl == rts) {
				print("Newton with bracketing", xacc, rts, j);
				return rts;
			}
		}
		else {
			dxold = dx;
			dx = f / df;
			temp = rts;
			rts -= dx;
			if (temp == rts) {
				print("Newton with bracketing", xacc, rts, j);
				return rts;
			}
		}
		if (fabs(dx) < xacc) {
			print("Newton with bracketing", xacc, rts, j);
			return rts;
		}
		(*funcd)(rts, &f, &df);
		if (f < 0.0)
			xl = rts;
		else
			xh = rts;
	}
	nrerror("Maximum number of iterations exceeded in rtsafe");
	return 0.0;
}


int main() {
	rtbis(F, 0, 400, 1e-4);
	rtbis(F, 0, 400, 1e-6);

	rtflsp(F, 0, 400, 1e-4);
	rtflsp(F, 0, 400, 1e-6);

	rtsec(F, 0, 400, 1e-4);
	rtsec(F, 0, 400, 1e-6);

	rtnewt(Fd, 0, 400, 1e-4);
	rtnewt(Fd, 0, 400, 1e-6);

	rtsafe(Fd, 0, 400, 1e-4);
	rtsafe(Fd, 0, 400, 1e-6);


	printf("F(325):\t\t%.10lf\n", F(325));
	printf("F(328):\t\t%.10lf\n", F(328));
	printf("F(328.151):\t%.10lf\n", F(328.151));
	printf("F(328.151429):\t%.10lf\n", F(328.151429));
}