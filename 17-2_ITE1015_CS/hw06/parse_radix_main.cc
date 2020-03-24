#include "parse_radix.h"

int main() {
	while (true) {
		string command;
		cin >> command;

		if (command == "eval") {
			bool error = false;

			string num1, oper, num2;
			int Pnum;
			cin >> num1 >> oper >> num2;

			Number N1 = Number(num1);
			Number N2 = Number(num2);

			N1.Nin10_ = ParseRadix(num1);
			N2.Nin10_ = ParseRadix(num2);

			string ans;

			if (N1.Nin10_ == -1 || N2.Nin10_ == -1) error = true;

			if (oper == "+") {
				cin >> Pnum;
				ans = ConvertRadix(Number(N1 + N2).Nin10_, Pnum);
			}
			else if (oper == "-") {
				cin >> Pnum;
				ans = ConvertRadix(Number(N1 - N2).Nin10_, Pnum);
			}
			else if (oper == "*") {
				cin >> Pnum;
				ans = ConvertRadix(Number(N1 * N2).Nin10_, Pnum);
			}
			else if (oper == "/") {
				cin >> Pnum;
				ans = ConvertRadix(Number(N1 / N2).Nin10_, Pnum);
			}
			else if (oper == "<") {
				if (oper1(N1, N2)) ans = "true";
				else ans = "false";
			}
			else if (oper == ">") {
				if (!oper1(N1, N2)) ans = "true";
				else ans = "false";
			}
			else if (oper == "==") {
				if (oper2(N1, N2)) ans = "true";
				else ans = "false";
			}

			if (error) cout << "Error" << endl;
			else cout << ans << endl;
		}
		else if (command == "quit") break;
	}
	return 0;
}