#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum { RECTANGLE, TRIANGLE_UP, TRIANGLE_DOWN };
enum { ERROR_OUT_OF_CANVAS = -1, ERROR_INVALID_INPUT = -2 };

struct Shape {
	int type;
	int x, y;
	int width, height;
	char brush;  // The character to draw the shape.
};

class Canvas {
public:
	Canvas(size_t row, size_t col);
	~Canvas();
	int AddShape(const Shape &s);  // Return the index of the shape.
	void DeleteShape(int index);
	void Draw(ostream& os);
	void Dump(ostream& os);

	void MakeCanvas();

private:
	size_t row_, col_;
	vector<Shape> shapes_;
	vector<string> canvas;
};
