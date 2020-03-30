#include <math.h>
#include <iostream>
using namespace std;

/*
ibeta : the floating-point radix (== B)
it : the number of base-ibeta digits in the floating-point mantissa M

machep : the exponent of the smallest (most negative) power of ibeta that, added to 1.0, gives something different from 1.0.
eps : the smallest positive number that, added to 1.0, is not equal to 1.0
negep : the exponent of the smallest power of ibeta that, subtracted from 1.0, gives something different from 1.0.
epsneg : the smallest positive number that, subtracted from 1.0, is not equal to 1.0

iexp : the number of bits in the exponent (including its sign or bias)
minexp : the smallest (most negative) power of ibeta consistent with there being no leading zeros in the mantissa
maxexp : the smallest (positive) power of ibeta that causes overflow.
xmin : the smallest representable positive number
xmax : the largest representable positive number

irnd : returns a code in the range 0...5, giving information on what kind of rounding is done in addition, and on how underflow is handled.
ngrd : the number of ¡°guard digits¡± used when truncating the product of two mantissas to fit the representation
*/

void machar_float(int *ibeta, int *it, int *machep, float *eps, int *negep, float *epsneg,
	int *iexp, int *minexp, int *maxexp, float *xmin, float *xmax, int *irnd, int *ngrd)
{
	int i, itemp, iz, j, k, mx, nxres;
	float a, b, beta, betah, betain, one, t, temp, temp1, tempa, two, y, z, zero;

	one = (float)(1);
	two = one + one;
	zero = one - one;

	a = one; // Determine ibeta and beta by the method of M
	do {
		a += a;
		temp = a + one;
		temp1 = temp - a;
	} while (temp1 - one == zero);
	b = one;
	do {
		b += b;
		temp = a + b;
		itemp = (int)(temp - a);
	} while (itemp == 0);
	*ibeta = itemp;
	beta = (float)(*ibeta);

	*it = 0; // Determine it and irnd.
	b = one;
	do {
		++(*it);
		b *= beta;
		temp = b + one;
		temp1 = temp - b;
	} while (temp1 - one == zero);
	*irnd = 0;
	betah = beta / two;
	temp = a + betah;
	if (temp - a != zero) *irnd = 1;
	tempa = a + beta;
	temp = tempa + betah;
	if (*irnd == 0 && temp - tempa != zero) *irnd = 2;

	*negep = (*it) + 3; // Determine negep and epsneg.
	betain = one / beta;
	a = one;
	for (i = 1; i <= (*negep); i++) a *= betain;
	b = a;
	for (;;) {
		temp = one - a;
		if (temp - one != zero) break;
		a *= beta;
		--(*negep);
	}
	*negep = -(*negep);
	*epsneg = a;
	*machep = -(*it) - 3; // Determine machep and eps.
	a = b;

	for (;;) {
		temp = one + a;
		if (temp - one != zero) break;
		a *= beta;
		++(*machep);
	}
	*eps = a;

	*ngrd = 0; // Determine ngrd.
	temp = one + (*eps);
	if (*irnd == 0 && temp*one - one != zero) *ngrd = 1;

	i = 0; // Determine iexp.
	k = 1;
	z = betain;
	t = one + (*eps);
	nxres = 0;
	for (;;) {
		// Loop until an underflow occurs, then exit.
		y = z;
		z = y * y;
		a = z * one; // Check here for the underflow.
		temp = z * t;
		if (a + a == zero || fabs(z) >= y) break;
		temp1 = temp * betain;
		if (temp1*beta == z) break;
		++i;
		k += k;
	}
	if (*ibeta != 10) {
		*iexp = i + 1;
		mx = k + k;
	}
	else {
		// For decimal machines only.
		*iexp = 2;
		iz = (*ibeta);
		while (k >= iz) {
			iz *= *ibeta;
			++(*iexp);
		}
		mx = iz + iz - 1;
	}
	for (;;) {
		// To determine minexp and xmin, loop until an
		*xmin = y; // underflow occurs, then exit.
		y *= betain;
		a = y * one; // Check here for the underflow.
		temp = y * t;
		if (a + a != zero && fabs(y) < *xmin) {
			++k;
			temp1 = temp * betain;
			if (temp1*beta == y && temp != y) {
				nxres = 3;
				*xmin = y;
				break;
			}
		}
		else break;
	}
	*minexp = -k; // Determine maxexp, xmax.
	if (mx <= k + k - 3 && *ibeta != 10) {
		mx += mx;
		++(*iexp);
	}
	*maxexp = mx + (*minexp);
	*irnd += nxres; // Adjust irnd to reflect partial underflow.
	if (*irnd >= 2) *maxexp -= 2; // Adjust for IEEE - style machines.
	i = (*maxexp) + (*minexp);
	// Adjust for machines with implicit leading bit in binary mantissa,
	// and machines with radix point at extreme right of mantissa.
	if (*ibeta == 2 && !i) --(*maxexp);
	if (i > 20) --(*maxexp);
	if (a != y) *maxexp -= 2;
	*xmax = one - (*epsneg);
	if ((*xmax)*one != *xmax) *xmax = one - beta * (*epsneg);
	*xmax /= (*xmin*beta*beta*beta);
	i = (*maxexp) + (*minexp) + 3;
	for (j = 1; j <= i; j++) {
		if (*ibeta == 2) *xmax += *xmax;
		else *xmax *= beta;
	}
}

void machar_double(int *ibeta, int *it, int *machep, double *eps, int *negep, double *epsneg,
	int *iexp, int *minexp, int *maxexp, double *xmin, double *xmax, int *irnd, int *ngrd)

{
	int i, itemp, iz, j, k, mx, nxres;
	double a, b, beta, betah, betain, one, t, temp, temp1, tempa, two, y, z, zero;
	one = (double)(1);
	two = one + one;
	zero = one - one;
	a = one; // Determine ibeta and beta by the method of M. Malcolm.
	do {
		a += a;
		temp = a + one;
		temp1 = temp - a;
	} while (temp1 - one == zero);
	b = one;
	do {
		b += b;
		temp = a + b;
		itemp = (int)(temp - a);
	} while (itemp == 0);
	*ibeta = itemp;
	beta = (double)(*ibeta);
	*it = 0; // Determine it and irnd.
	b = one;
	do {
		++(*it);
		b *= beta;
		temp = b + one;
		temp1 = temp - b;
	} while (temp1 - one == zero);
	*irnd = 0;
	betah = beta / two;
	temp = a + betah;
	if (temp - a != zero) *irnd = 1;
	tempa = a + beta;
	temp = tempa + betah;
	if (*irnd == 0 && temp - tempa != zero) *irnd = 2;
	*negep = (*it) + 3; // Determine negep and epsneg.
	betain = one / beta;
	a = one;
	for (i = 1; i <= (*negep); i++) a *= betain;
	b = a;
	for (;;) {
		temp = one - a;
		if (temp - one != zero) break;
		a *= beta;
		--(*negep);
	}
	*negep = -(*negep);
	*epsneg = a;
	*machep = -(*it) - 3; // Determine machep and eps.
	a = b;

	for (;;) {
		temp = one + a;
		if (temp - one != zero) break;
		a *= beta;
		++(*machep);
	}
	*eps = a;
	*ngrd = 0; // Determine ngrd.
	temp = one + (*eps);
	if (*irnd == 0 && temp*one - one != zero) *ngrd = 1;
	i = 0; // Determine iexp.
	k = 1;
	z = betain;
	t = one + (*eps);
	nxres = 0;
	for (;;) {
		// Loop until an underflow occurs, then exit.
		y = z;
		z = y * y;
		a = z * one; // Check here for the underflow.
		temp = z * t;
		if (a + a == zero || fabs(z) >= y) break;
		temp1 = temp * betain;
		if (temp1*beta == z) break;
		++i;
		k += k;
	}
	if (*ibeta != 10) {
		*iexp = i + 1;
		mx = k + k;
	}
	else {
		// For decimal machines only.
		*iexp = 2;
		iz = (*ibeta);
		while (k >= iz) {
			iz *= *ibeta;
			++(*iexp);
		}
		mx = iz + iz - 1;
	}
	for (;;) {
		// To determine minexp and xmin, loop until an
		*xmin = y; // underflow occurs, then exit.
		y *= betain;
		a = y * one; // Check here for the underflow.
		temp = y * t;
		if (a + a != zero && fabs(y) < *xmin) {
			++k;
			temp1 = temp * betain;
			if (temp1*beta == y && temp != y) {
				nxres = 3;
				*xmin = y;
				break;
			}
		}
		else break;
	}
	*minexp = -k; // Determine maxexp, xmax.
	if (mx <= k + k - 3 && *ibeta != 10) {
		mx += mx;
		++(*iexp);
	}
	*maxexp = mx + (*minexp);
	*irnd += nxres; // Adjust irnd to reflect partial underflow.
	if (*irnd >= 2) *maxexp -= 2; // Adjust for IEEE - style machines.
	i = (*maxexp) + (*minexp);
	// Adjust for machines with implicit leading bit in binary mantissa, and machines with radix point at extreme right of mantissa.
	if (*ibeta == 2 && !i) --(*maxexp);
	if (i > 20) --(*maxexp);
	if (a != y) *maxexp -= 2;
	*xmax = one - (*epsneg);
	if ((*xmax)*one != *xmax) *xmax = one - beta * (*epsneg);
	*xmax /= (*xmin*beta*beta*beta);
	i = (*maxexp) + (*minexp) + 3;
	for (j = 1; j <= i; j++) {
		if (*ibeta == 2) *xmax += *xmax;
		else *xmax *= beta;
	}
}

void test_float() {
	int ibeta, it, machep, negep, iexp, minexp, maxexp, irnd, ngrd;
	float eps, epsneg, xmin, xmax;

	machar_float(&ibeta, &it, &machep, &eps, &negep, &epsneg,
		&iexp, &minexp, &maxexp, &xmin, &xmax, &irnd, &ngrd);

	cout << "	IBETA =	 " << ibeta << "\n";
	cout << "	IT =	 " << it << "\n";
	cout << "	IRND =	 " << irnd << "\n";
	cout << "	NGRD =	 " << ngrd << "\n";
	cout << "	MACHEP = " << machep << "\n";
	cout << "	NEGEP =	 " << negep << "\n";
	cout << "	IEXP =	 " << iexp << "\n";
	cout << "	MINEXP = " << minexp << "\n";
	cout << "	MAXEXP = " << maxexp << "\n";
	cout << "	EPS =	 " << eps << "\n";
	cout << "	EPSNEG = " << epsneg << "\n";
	cout << "	XMIN =	 " << xmin << "\n";
	cout << "	XMAX =	 " << xmax << "\n";
}

void test_double() {
	int ibeta, it, machep, negep, iexp, minexp, maxexp, irnd, ngrd;
	double eps, epsneg, xmin, xmax;

	machar_double(&ibeta, &it, &machep, &eps, &negep, &epsneg,
		&iexp, &minexp, &maxexp, &xmin, &xmax, &irnd, &ngrd);

	cout << "	IBETA =	 " << ibeta << "\n";
	cout << "	IT =	 " << it << "\n";
	cout << "	IRND =	 " << irnd << "\n";
	cout << "	NGRD =	 " << ngrd << "\n";
	cout << "	MACHEP = " << machep << "\n";
	cout << "	NEGEP =	 " << negep << "\n";
	cout << "	IEXP =	 " << iexp << "\n";
	cout << "	MINEXP = " << minexp << "\n";
	cout << "	MAXEXP = " << maxexp << "\n";
	cout << "	EPS =	 " << eps << "\n";
	cout << "	EPSNEG = " << epsneg << "\n";
	cout << "	XMIN =	 " << xmin << "\n";
	cout << "	XMAX =	 " << xmax << "\n";
}

int main(){
	cout << "Obtain the machine accuracy of my computer by using the routine 'machar()'\n";

	cout << "\nThe machine accuracy of 'float'\n";
	test_float();

	cout << "\nThe machine accuracy of 'double'\n";
	test_double();

	return 0;
}



