#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Canvas {
public:
	Canvas(size_t row, size_t col);
	~Canvas();

	// Canvas 크기를 w x h 로 변경한다. 그려진 내용은 보존한다.
	void Resize(size_t w, size_t h);

	// (x,y) 위치에 ch 문자를 그린다. 범위 밖의 x,y 는 무시한다.
	bool Draw(int x, int y, char brush);

	// 그려진 내용을 모두 지운다 ('.'으로 초기화)
	void Clear();

private:// 그려진 모양을 저장할 수 있도록 데이터멤버를 정의 (resize 가능에 주의)
	size_t row_, col_;
	vector<string> canvas;
	friend ostream& operator<<(ostream& os, const Canvas& c);
};

class Shape {
public:
	virtual ~Shape();
	virtual void Draw(Canvas* canvas) const = 0;

	string type();
	int x(), y(), w(), h();
	char brush();
	virtual void set(string t, int x, int y, int w, int h, char b);

protected: // 도형의 공통 속성을 정의.
	string type_;
	int x_, y_;
	int w_;
	int h_;
	char b_;
};

class Rectangle : public Shape {
	void Draw(Canvas* canvas) const;
};
class UpTriangle : public Shape { 
	void Draw(Canvas* canvas) const;
};
class DownTriangle : public Shape {
	void Draw(Canvas* canvas) const;
};
class Diamond : public Shape {
	void Draw(Canvas* canvas) const;
};

istream& operator>>(istream& is, Rectangle& r);
istream& operator>>(istream& is, UpTriangle& t);
istream& operator>>(istream& is, DownTriangle& d);
istream& operator>>(istream& is, Diamond& dm);
