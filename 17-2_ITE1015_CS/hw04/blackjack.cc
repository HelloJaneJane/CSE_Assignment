#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctype.h>
using namespace std;

int argc = 0;
int ans = 0;
int BlackJack(string* argv) {
	int Acnt = 0;

	int cnt = atoi(argv[0].c_str());
	if (cnt <= 0||cnt!=argc-1) return ans = -1;

	for (int i = 1; i<argc; i++) {
		string arg = argv[i];
		if (isdigit(arg[arg.length() - 1]))	ans = ans + atoi(arg.c_str());
		else if (arg == "J" || arg == "Q" || arg == "K")	ans = ans + 10;
		else if (arg == "A")	Acnt++;
		else return ans = -1;
	}

	if (Acnt>0) {
		if (ans >= 11) ans = ans + Acnt;
		else {
			if (Acnt == 1) ans = ans + 11;
			else ans = ans + 10 + Acnt;
		}
	}
	return ans;
}

void print(int ans) {
	if (ans<21) cout << ans << endl;
	else if (ans == 21) cout << "BlackJack" << endl;
	else if (ans>21) cout << "Exceed" << endl;
}

int main() {
	while (true) {
		string inputs;
		getline(cin, inputs);

		if (inputs == "q") break;

		string *argv = new string[inputs.length()];
		string token;
		size_t pos;
		argc = 0;
		ans = 0;

		while (true) {
			pos = inputs.find(' ');
			token = inputs.substr(0, pos);
			argv[argc++] = token;

			if (pos >= inputs.length()) break;
			else inputs.erase(0, pos + 1);
		}

		BlackJack(argv);
		if (ans <= 0) break;
		else print(ans);
	}
	return 0;
}
