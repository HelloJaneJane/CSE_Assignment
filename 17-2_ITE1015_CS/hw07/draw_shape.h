#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Canvas {
public:
	Canvas(size_t row, size_t col);
	~Canvas();

	// Canvas ũ�⸦ w x h �� �����Ѵ�. �׷��� ������ �����Ѵ�.
	void Resize(size_t w, size_t h);

	// (x,y) ��ġ�� ch ���ڸ� �׸���. ���� ���� x,y �� �����Ѵ�.
	bool Draw(int x, int y, char brush);

	// �׷��� ������ ��� ����� ('.'���� �ʱ�ȭ)
	void Clear();

private:// �׷��� ����� ������ �� �ֵ��� �����͸���� ���� (resize ���ɿ� ����)
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

protected: // ������ ���� �Ӽ��� ����.
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
