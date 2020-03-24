#include "minesweeper.h"
#include <iostream>
using namespace std;


Minesweeper::Minesweeper() {
	map.push_back("");
}

Minesweeper::~Minesweeper() {
	map.clear();
}

size_t Minesweeper::width() const {
	return map[0].length();
}

size_t Minesweeper::height() const {
	return map.size();
}

char Minesweeper::get(int _x, int _y) const {
	if (_x >= int(width()) || _y >= int(height()) || _x < 0 || _y < 0) return ' ';
	else return map[_y].at(_x);
}


bool Minesweeper::touchMap(int _x, int _y) {
	int h = map.size();
	int w = map[0].length();
	if (_x < 0 || _x >= w || _y < 0 || _y >= h) return false;

	touchCnt++;
	char c = get(_x, _y);

	if (c == '*') {
		cout << "DEAD(" << touchCount() << ")" << endl;
		return false;
	}
	else if (isdigit(c)) {
		map2[_y].at(_x) = c;
		for (int i = 0; i < int(map2.size()); i++) {
			cout << map2[i] << endl;
		}
	}
	return true;
}

int Minesweeper::touchCount() const {
	return touchCnt;
}

