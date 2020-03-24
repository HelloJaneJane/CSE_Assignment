#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

struct Point {
	int x_, y_;  // 멤버 변수.
	string name_;

	Point();
	Point(const Point& p);
	explicit Point(int c);
	Point(int x, int y);

	Point operator-() const;  // 전위 - 연산자
};

Point operator+(const Point& lhs, const Point& rhs);
Point operator-(const Point& lhs, const Point& rhs);
Point operator*(const Point& lhs, const Point& rhs);

void print(Point A);
Point Oper(string oper, const Point& lhs, const Point& rhs);
