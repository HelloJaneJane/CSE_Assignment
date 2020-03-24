#include "draw_shape.h"

Canvas::Canvas(size_t row, size_t col) {
	row_ = row;
	col_ = col;
}
Canvas::~Canvas() {
	shapes_.clear();
	canvas.clear();
}

void Canvas::MakeCanvas() {
	string line;
	for (int i = 0; i < row_; i++) {
		for (int j = 0; j < col_; j++) {
			line+= ".";
		}
		canvas.push_back(line);
		line = "";
	}
}


int Canvas::AddShape(const Shape &s) {
	if (s.type == 0) {                                       // �簢���� ���
		if (s.width % 2 == 0 || s.height % 2 == 0) {             // �Է��� �߸��� ���
			cout << "error invalid input" << endl;
			return -2;
		}
		if (s.x + s.width / 2 >= col_ || s.x - (s.width / 2) < 0 || s.y + (s.height / 2) >= row_ || s.y - (s.height / 2) < 0 || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {      // ��� ���� Ȯ��
			cout << "error out of canvas" << endl;
			return -1;
		}
	}
	else if (s.type == 1) {
		if (s.y - s.height + 1 < 0 || s.x - (2 * s.height - 1) / 2 < 0 || s.x + (2 * s.height - 1) / 2 >= col_ || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {                     // ��� ���� Ȯ��
			cout << "error out of canvas" << endl;
			return -1;
		}
	}
	else if (s.type == 2) {
		if (s.y + s.height - 1 > row_ || s.x - (2 * s.height - 1) / 2 < 0 || s.x + (2 * s.height - 1) / 2 >= col_ || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {                    // ��� ���� Ȯ��
			cout << "error out of canvas" << endl;
			return -1;
		}
	}

	shapes_.push_back(s);                                               // ��� �־��ֱ�

	return shapes_.size();
}
void Canvas::DeleteShape(int index){
	vector<Shape>::iterator it = shapes_.begin();
	if (shapes_.size() > index) {                                       //   �ε����� ��簳������ ���� ���
		for (int i = 0; i != index; i++) {                                //   �ε��� ã�Ƽ� �����
			it++;
		}
		shapes_.erase(it);
	}
}
void Canvas::Draw(ostream& os) {
	int type, x, y, width, height;
	char point;
	int count = 0;
	string line;
	if (shapes_.empty()) {                                       // �Է¹��� ����� ���� ���
		cout << " ";
		for (int i = 0; i< col_; i++) {                         // �� ��ȣ
			int k;
			k = i;
			if (k >= 10)     k -= 10;
			cout << k;
		}
		cout << endl;
		for (vector<string>::iterator it = canvas.begin(); count < row_; it++) {  // �� ��ȣ�� ��ȭ�� ���
			os << count << *it << endl;
			count++;
		}
	}
	else {
		for (int i = 0; i < shapes_.size(); i++) {                // ��� ������ŭ �߰�

			type = shapes_[i].type;                             // ��� ���� Ȯ��

			if (type == 0) {
				x = shapes_[i].x;   y = shapes_[i].y;   width = shapes_[i].width;   height = shapes_[i].height;
				point = shapes_[i].brush;

				for (int i = y - (height / 2); i <= y + (height / 2); i++) {         // �Է¹��� �귯���� ��� �����
					for (int j = x - (width / 2); j <= x + (width / 2); j++) {
						canvas[i][j] = point;
					}
				}
			}

			else if (type == 1) {                            // ��� ���� Ȯ��
				x = shapes_[i].x;   y = shapes_[i].y;   height = shapes_[i].height;
				point = shapes_[i].brush;

				int k = 0;
				for (int i = y; i< y + height; i++) {        // �Է¹��� �귯���� ��� �����
					for (int j = x - k; j <= x + k; j++) {
						canvas[i][j] = point;
					}
					k++;
				}
			}
			else if (type == 2) {                            // ��� ���� Ȯ��
				x = shapes_[i].x;   y = shapes_[i].y;   height = shapes_[i].height;
				point = shapes_[i].brush;

				int k = 0;
				for (int i = y; i > y - height; i--) {       // �Է¹��� �귯���� ��� �����
					for (int j = x - k; j <= x + k; j++) {
						canvas[i][j] = point;
					}
					k++;
				}
			}
		}

		cout << " ";
		for (int i = 0; i<col_; i++) {                         // �� ��ȣ ���
			int k;
			k = i;
			if (k >= 10)  k -= 10;
			cout << k;
		}
		cout << endl;
		for (vector<string>::iterator it = canvas.begin(); count < row_; it++) {  // �� ��ȣ�� �׷��� ��ȭ�� ���
			os << count << *it << endl;
			count++;
		}

	}
	canvas.clear();
}
void Canvas::Dump(ostream& os) {
	int i = 0;
	for (vector<Shape>::iterator it = shapes_.begin(); it != shapes_.end(); it++) {
		string type;
		if (it->type == 0) {                                            // �簢���� ���
			type = "rect";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->width << " " << it->height << " " << it->brush << endl;
			i++;
		}
		if (it->type == 1) {                                             // �ﰢ���� ���
			type = "tri_up";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->height << " " << it->brush << endl;
			i++;
		}
		if (it->type == 2) {                                             // ���ﰢ���� ���
			type = "tri_down";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->height << " " << it->brush << endl;
			i++;
		}

	}
}
