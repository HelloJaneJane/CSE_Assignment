#include "draw_shape.h"

Canvas::Canvas(size_t row, size_t col) {
	row_ = row;
	col_ = col;
	Clear();
}
Canvas::~Canvas() {
	canvas.clear();
}

void Canvas::Resize(size_t w, size_t h) {
	row_ = w;
	col_ = h;
	Clear();
}

bool Canvas::Draw(int x, int y, char brush) {
	if (x< row_&&y < col_) {
		(canvas[y])[x] = brush;
		return true;
	}
	return false;
}

void Canvas::Clear() {
	canvas.clear();
	string line;
	for (int j = 0; j < (int)row_; j++) {
		line += ".";
	}
	for (int i = 0; i < (int)col_; i++) {
		canvas.push_back(line);
	}
}

Shape::~Shape(){
}
void Shape::Draw(Canvas* canvas) const {

}

string Shape::type() {
	return type_;
}
int Shape::x() {
	return x_;
}
int Shape::y() {
	return y_;
}
int Shape::w() {
	return w_;
}
int Shape::h() {
	return h_;
}
char Shape::brush() {
	return b_;
}

void Rectangle::Draw(Canvas* canvas) const{
	for (int i = 0; i < w_; i++) {
		for (int j = 0; j < h_; j++) {
			canvas->Draw(x_ + i, y_ + j, b_);
		}
	}
}
void DownTriangle::Draw(Canvas* canvas) const {
	for (int j = 0; j < h_; j++) {
		for (int i = -1 * j; i <= j; i++) {
			canvas->Draw(x_ + i, y_ - j, b_);
		}
	}
}
void UpTriangle::Draw(Canvas* canvas) const {
	for (int j = 0; j < h_; j++) {
		for (int i = -1 * j; i <= j; i++) {
			canvas->Draw(x_ + i, y_ + j, b_);
		}
	}
}
void Diamond::Draw(Canvas* canvas) const {
	for (int j = 0; j <= h_; j++) {
		for (int i = -1 * j; i <= j; i++) {
			canvas->Draw(x_ + i, y_ + j, b_);
		}
	}
	int y__ = y_ + 2 * h_;
	for (int j = 0; j < h_; j++) {
		for (int i = -1 * j; i <= j; i++) {
			canvas->Draw(x_ + i, y__ - j, b_);
		}
	}
}

void Shape:: set(string t, int x, int y, int w, int h, char b) {
	type_ = t;
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;
	b_ = b;
}

ostream& operator<<(ostream& os, const Canvas& c) {
	os << " ";
	for (int i = 0; i < (int)c.row_; i++) {
		os << i%10;
	}
	os << endl;
	for (int i = 0; i < (int)c.col_; i++) {
		os << i % 10 << c.canvas[i] << endl;
	}
	return os;
}

istream& operator>>(istream& is, Rectangle& r) {
	int x, y, w, h;
	char b;
	is >> x >> y >> w >> h >> b;
	r.set("rect", x, y, w, h, b);
	return is;
}
istream& operator>>(istream& is, UpTriangle& t) {
	int x, y, h;
	char b;
	is >> x >> y >> h >> b;
	t.set("tri_up", x, y, 0, h, b);
	return is;
}
istream& operator>>(istream& is, DownTriangle& d) {
	int x, y, h;
	char b;
	is >> x >> y >> h >> b;
	d.set("tri_down", x, y, 0, h, b);
	return is;
}
istream& operator>>(istream& is, Diamond& dm) {
	int x, y, h;
	char b;
	is >> x >> y >> h >> b;
	dm.set("diamond", x, y, 0, h, b);
	return is;
}
