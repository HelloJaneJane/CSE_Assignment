#include <iostream>
using namespace std;

float get_eps_float() {
	float add = 1.0;
	float tmp;
	float eps;

	do {
		add /= 2.0;
		tmp = add + 1.0;
		tmp -= 1.0;

		if (tmp > 0.0) eps = tmp;
	} while (tmp > 0.0);

	return eps;
}

double get_eps_double() {
	double add = 1.0;
	double tmp = add / 2.0 + 1.0;
	double eps;

	do {
		add /= 2.0;
		tmp = add + 1.0;
		tmp -= 1.0;

		if (tmp > 0.0) eps = tmp;
	} while (tmp > 0.0);

	return eps;
}

int main() {
	cout << "Obtain the machine accuracy of my computer by using my own code 'get_eps()'\n";

	cout << "\nThe machine accuracy of 'float'\n";
	cout << get_eps_float() << "\n";

	cout << "\nThe machine accuracy of 'double'\n";
	cout << get_eps_double() << "\n";

	return 0;
}
