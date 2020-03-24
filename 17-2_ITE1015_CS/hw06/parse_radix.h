#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

int ParseRadix(const string& number);
string ConvertRadix(int number, int radix);

struct Number {
	string N_R;
	int Nin10_;

	Number();
	Number(string N_R);
	Number(int Nin10);
};

Number operator+(const Number& num1, const Number& num2);
Number operator-(const Number& num1, const Number& num2);
Number operator*(const Number& num1, const Number& num2);
Number operator/(const Number& num1, const Number& num2);
bool oper1(const Number& num1, const Number& num2);//true : < , false : >
bool oper2(const Number& num1, const Number& num2);//true : == , false : !=