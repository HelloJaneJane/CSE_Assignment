#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

struct Point {
	int x_, y_;  // ��� ����.
	string name_;

	Point();
	Point(const Point& p);
	explicit Point(int c);
	Point(int x, int y);

	Point operator-() const;  // ���� - ������
};

Point operator+(const Point& lhs, const Point& rhs);
Point operator-(const Point& lhs, const Point& rhs);
Point operator*(const Point& lhs, const Point& rhs);

void print(Point A);
Point Oper(string oper, const Point& lhs, const Point& rhs);
