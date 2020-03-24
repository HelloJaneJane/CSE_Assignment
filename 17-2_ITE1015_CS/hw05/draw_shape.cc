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
	if (s.type == 0) {                                       // 사각형인 경우
		if (s.width % 2 == 0 || s.height % 2 == 0) {             // 입력이 잘못된 경우
			cout << "error invalid input" << endl;
			return -2;
		}
		if (s.x + s.width / 2 >= col_ || s.x - (s.width / 2) < 0 || s.y + (s.height / 2) >= row_ || s.y - (s.height / 2) < 0 || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {      // 벗어난 조건 확인
			cout << "error out of canvas" << endl;
			return -1;
		}
	}
	else if (s.type == 1) {
		if (s.y - s.height + 1 < 0 || s.x - (2 * s.height - 1) / 2 < 0 || s.x + (2 * s.height - 1) / 2 >= col_ || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {                     // 벗어난 조건 확인
			cout << "error out of canvas" << endl;
			return -1;
		}
	}
	else if (s.type == 2) {
		if (s.y + s.height - 1 > row_ || s.x - (2 * s.height - 1) / 2 < 0 || s.x + (2 * s.height - 1) / 2 >= col_ || s.x < 0 || s.y < 0 || s.x >= col_ || s.y >= row_) {                    // 벗어난 조건 확인
			cout << "error out of canvas" << endl;
			return -1;
		}
	}

	shapes_.push_back(s);                                               // 모양 넣어주기

	return shapes_.size();
}
void Canvas::DeleteShape(int index){
	vector<Shape>::iterator it = shapes_.begin();
	if (shapes_.size() > index) {                                       //   인덱스가 모양개수보다 적은 경우
		for (int i = 0; i != index; i++) {                                //   인덱스 찾아서 지우기
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
	if (shapes_.empty()) {                                       // 입력받은 모양이 없는 경우
		cout << " ";
		for (int i = 0; i< col_; i++) {                         // 열 번호
			int k;
			k = i;
			if (k >= 10)     k -= 10;
			cout << k;
		}
		cout << endl;
		for (vector<string>::iterator it = canvas.begin(); count < row_; it++) {  // 행 번호와 도화지 출력
			os << count << *it << endl;
			count++;
		}
	}
	else {
		for (int i = 0; i < shapes_.size(); i++) {                // 모양 개수만큼 추가

			type = shapes_[i].type;                             // 모양 종류 확인

			if (type == 0) {
				x = shapes_[i].x;   y = shapes_[i].y;   width = shapes_[i].width;   height = shapes_[i].height;
				point = shapes_[i].brush;

				for (int i = y - (height / 2); i <= y + (height / 2); i++) {         // 입력받은 브러쉬로 모양 만들기
					for (int j = x - (width / 2); j <= x + (width / 2); j++) {
						canvas[i][j] = point;
					}
				}
			}

			else if (type == 1) {                            // 모양 종류 확인
				x = shapes_[i].x;   y = shapes_[i].y;   height = shapes_[i].height;
				point = shapes_[i].brush;

				int k = 0;
				for (int i = y; i< y + height; i++) {        // 입력받은 브러쉬로 모양 만들기
					for (int j = x - k; j <= x + k; j++) {
						canvas[i][j] = point;
					}
					k++;
				}
			}
			else if (type == 2) {                            // 모양 종류 확인
				x = shapes_[i].x;   y = shapes_[i].y;   height = shapes_[i].height;
				point = shapes_[i].brush;

				int k = 0;
				for (int i = y; i > y - height; i--) {       // 입력받은 브러쉬로 모양 만들기
					for (int j = x - k; j <= x + k; j++) {
						canvas[i][j] = point;
					}
					k++;
				}
			}
		}

		cout << " ";
		for (int i = 0; i<col_; i++) {                         // 열 번호 출력
			int k;
			k = i;
			if (k >= 10)  k -= 10;
			cout << k;
		}
		cout << endl;
		for (vector<string>::iterator it = canvas.begin(); count < row_; it++) {  // 행 번호와 그려진 도화지 출력
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
		if (it->type == 0) {                                            // 사각형인 경우
			type = "rect";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->width << " " << it->height << " " << it->brush << endl;
			i++;
		}
		if (it->type == 1) {                                             // 삼각형인 경우
			type = "tri_up";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->height << " " << it->brush << endl;
			i++;
		}
		if (it->type == 2) {                                             // 역삼각형인 경우
			type = "tri_down";
			os << i << " " << type << " " << it->x << " " << it->y << " " << it->height << " " << it->brush << endl;
			i++;
		}

	}
}
