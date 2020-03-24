#include "minesweeper.h"
#include <iostream>
using namespace std;

int x, y = 0;
int dx[8] = { -1,-1,-1,0,1,1,1,0 };
int dy[8] = { 1,0,-1,-1,-1,0,1,1 };

void returnXY(string inputs, int n) {
	inputs.erase(0, n);
	int number = -1;

	for (int i = 0; i < int(inputs.size()); ++i) {
		char c = inputs[i];

		if (isdigit(c))
		{
			int num = (int)c - (int)'0';
			if (number < 0) number = num;
			else number = number * 10 + num;
		}
		else if (c == ' ') {
			x = number;
			number = -1;
		}
	}
	y = number;
}


bool Minesweeper::setMap(size_t _width, size_t _height, vector<string>& _map) {
	for (int i = 0; i < int(_height); i++) {
		string s = _map[i];

		if (s.length() != _width) return false;

		for (int j = 0; j < int(_width); j++) {
			if (s[j] != '*' && s[j] != '.') return false;
		}
	}

	for (int _y = 0; _y<int(_height); _y++) {
		for (int _x = 0; _x<int(_width); _x++) {
			char c = _map[_y].at(_x);
			int num = 0;
			if (c == '.') {
				for (int a = 0; a < 8; a++) {
					int new_x = _x + dx[a];
					int new_y = _y + dy[a];
					if (new_x < 0 || new_x >= x || new_y<0 || new_y >= y) continue;
					char new_c = _map[new_y].at(new_x);
					if (new_c == '*') num++;
				}
				_map[_y].at(_x) = num + 48;
			}
		}
	}
	map.clear();
	map = _map;

	for (int i = 0; i < y; i++) {
		cout << map[i] << endl;
	}

	return true;
}

bool Minesweeper::toggleMine(int _x, int _y) {
	int h = map.size();
	int w = map[0].length();
	if (_x < 0 || _x >= w || _y < 0 || _y >= h) return false;

	char c = get(_x, _y);
	if (isdigit(c)) {
		map[_y].at(_x) = '*';

		//주변숫자올리기
		for (int i = 0; i < 8; i++) {
			int new_x = _x + dx[i];
			int new_y = _y + dy[i];
			if (new_x < 0 || new_x >= w || new_y < 0 || new_y >= h) continue;

			char new_c = map[new_y].at(new_x);

			if (new_c == '*') continue;
			else if (isdigit(new_c)) {
				int newnew_c = new_c - '0' + 1;
				map[new_y].at(new_x) = newnew_c + '0';
			}
		}
	}
	else if (c == '*') {
		//주변숫자내리기
		int num = 0;
		for (int i = 0; i < 8; i++) {
			int new_x = _x + dx[i];
			int new_y = _y + dy[i];
			if (new_x < 0 || new_x >= w || new_y<0 || new_y >= h) continue;

			char new_c = map[new_y].at(new_x);

			if (new_c == '*') {
				num++;
				continue;
			}
			else if (isdigit(new_c)) {
				int newnew_c = new_c - '0' - 1;
				map[new_y].at(new_x) = newnew_c + '0';
			}
		}
		(map.at(_y)).at(_x) = num + '0';
	}
	else return false;

	int a = map.size();
	for (int i = 0; i < a; i++) {
		cout << map[i] << endl;
	}

	return true;
}

bool Minesweeper::setPlay() {
	//if (Pstatus == false) return false;

	for (int i = 0; i < y; i++) {
		string s = "";
		for (int j = 0; j < x; j++) {
			s += "_";
		}
		map2.push_back(s);
	}
	touchCnt = 0;
	for (int i = 0; i < y; i++) {
		cout << map2[i] << endl;
	}
	return true;
}

vector<string> new_map;
bool Tstatus, Pstatus = false;

int main() {
	Minesweeper *MAP = new Minesweeper();

	while (true) {
		string inputs;
		getline(cin, inputs);

		if (inputs == ":quit") break;

		else if (inputs.find(":set") != string::npos)
		{
			returnXY(inputs, 5);
			new_map.clear();
			for (int i = 0; i < y; i++) {
				string s;
				cin >> s;
				new_map.push_back(s);
			}
			Tstatus = true;
			Pstatus = true;
			MAP->setMap(x, y, new_map);
		}

		else if (inputs.find(":toggle") != string::npos)
		{
			returnXY(inputs, 8);
			Pstatus = false;
			if (Tstatus) MAP->toggleMine(x, y);
		}

		else if (inputs == ":play") {
			Tstatus = false;
			if (Pstatus) MAP->setPlay();
		}

		else if (inputs.find(":touch") != string::npos)
		{
			returnXY(inputs, 7);
			if (Pstatus) MAP->touchMap(x, y);
		}

	}

	delete MAP;
	return 0;
}