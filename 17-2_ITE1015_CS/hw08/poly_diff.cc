#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

int main() {
	while (true) {
		string input;
		cin >> input;
		if (input == "quit") break;
		else {
			int n;
			cin >> n;

			vector <string> v1; // ��
			vector <pair<int, int>> v2; // <���,����>
			
			int l = input.length();
			int plus = -1;
			for (int i = 0; i < l; i++) {
				if (input[i] == '+') {
					v1.push_back(input.substr(plus + 1, i - plus - 1));
					plus = i;
				}
				if (i == l - 1)	v1.push_back(input.substr(plus + 1, i - plus));
			}

			vector<string>::iterator it1;
			for (it1 = v1.begin(); it1 != v1.end(); it1++) {
				string s = *it1;
				int l, r;
				int x = s.find("x");
				
				if (x == -1) {// x������ �����
					r = 0;
					l = stoi(s);
				}
				else {//x�� ������				
					if (x == 0) l = 1; // x���Ǿ��̸� ���1
					else l = stoi(s.substr(0, x)); // �Ǿ��̾ƴҶ�

					int caret = s.find("^");
					if (caret == -1) r = 1; // ^������ ����1	
					else r = stoi(s.substr(caret + 1, s.length() - caret)); //^������
				}
				v2.push_back({ l,r });
			}
			
			int ans = 0;
			vector<pair<int, int>>::iterator it2;
			for (it2 = v2.begin(); it2 != v2.end(); it2++) {
				pair<int, int> p = *it2;
				ans += (p.first)*(p.second)*pow(n, p.second - 1);
			}
			cout << ans << endl;
		}
	}
	return 0;
}