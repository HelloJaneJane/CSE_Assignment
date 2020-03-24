#include "point2d.h"

Point::Point() {

}
Point::Point(const Point& p) { //copy
	x_ = p.x_;
	y_ = p.y_;
}
/*explicit*/ Point::Point(int c){
	x_ = c;
	y_ = c;
}
Point::Point(int x, int y) {
	x_ = x;
	y_ = y;
}

Point Point::operator-() const{  // 전위 - 연산자
	return Point(-x_, -y_);
}

Point operator+(const Point& lhs, const Point& rhs){
	return Point(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
}
Point operator-(const Point& lhs, const Point& rhs) {
	return Point(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_);
}
Point operator*(const Point& lhs, const Point& rhs) {
	return Point(lhs.x_ * rhs.x_, lhs.y_ * rhs.y_);
}

void print(Point A) {
	cout << "(" << A.x_ << ", " << A.y_ << ")" << endl;
}

Point Oper(string oper, const Point& lhs, const Point& rhs){
	if (oper == "+") return Point(lhs + rhs);
	else if (oper == "-") return Point(lhs - rhs);
	else if (oper == "*") return Point(lhs * rhs);
}