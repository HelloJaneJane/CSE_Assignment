#include "parse_radix.h"

int ParseRadix(const string& number) {
	// "a_b" 는 b진법으로 표현된 a
	// 얘를 10진법으로 바꾼 값을 리턴하는 함수

	int bar = number.find('_');

	if (bar < 0) { //10진법이라 _b 를 안쓴경우
		return stoi(number);
	}
	else {
		string numstr = number.substr(0, bar);
		int rdx = stoi(number.substr(bar + 1));
		int ans;
		try { ans = stoi(numstr, 0, rdx); }
		catch (invalid_argument e) { return -1; }
		return ans;
	}
}

string ConvertRadix(int number, int radix) {
	//10진수 number를 radix진수로 바꾼 거 리턴
	//radix가 10이면 그냥 number만, 아니면 뒤에 "_radix" 붙여야함

	bool minus = false;
	if (number < 0) {
		minus = true;
		number = -1 * number;
	}

	char arr[2000];
	int cnt = 0;
	while (number>0) {
		int a = number%radix;
		if (a<10) {
			arr[cnt] = a + 48;
		}
		else if (a >= 10) {
			arr[cnt] = a + 87;
		}
		number = number / radix;
		cnt++;
	}
	string ans = "";
	if (minus)ans += "-";
	while (cnt>0) {
		ans += arr[cnt - 1];
		cnt--;
	}
	if (radix != 10) ans = ans + "_" + to_string(radix);
	return ans;
}

Number::Number() {
}
Number::Number(string input) {
	N_R = input;
}
Number::Number(int Nin10) {
	Nin10_ = Nin10;
}

Number operator+(const Number& num1, const Number& num2) {
	return Number(num1.Nin10_ + num2.Nin10_);
}
Number operator-(const Number& num1, const Number& num2) {
	return Number(num1.Nin10_ - num2.Nin10_);
}
Number operator*(const Number& num1, const Number& num2) {
	return Number(num1.Nin10_ * num2.Nin10_);
}
Number operator/(const Number& num1, const Number& num2) {
	return Number(num1.Nin10_ / num2.Nin10_);
}

bool oper1(const Number& num1, const Number& num2) {
	if (num1.Nin10_ < num2.Nin10_) return true;
	else return false;
}
bool oper2(const Number& num1, const Number& num2) {
	if (num1.Nin10_ == num2.Nin10_) return true;
	else return false;
}