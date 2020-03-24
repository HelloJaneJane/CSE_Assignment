#include "draw_shape.h"

int main() {
	size_t row, col;
	cin >> row >> col;
	
	Shape mydohyung;
	Canvas *mycanvas = new Canvas(row, col);
	
	string command, dohyung;
	int idx;

	bool first = true;
	
	while (true) {
		mycanvas->MakeCanvas();
		if (first) {    
			mycanvas->Draw(cout);
			first = false;
		}

		cin >> command;

		if (command == "quit") break;
		else if (command == "add") {
			cin >> dohyung;

			if (dohyung == "rect") {
				mydohyung.type = 0;
				cin >> mydohyung.x >> mydohyung.y >> mydohyung.width >> mydohyung.height >> mydohyung.brush;
				mycanvas->AddShape(mydohyung);

			}
			else if (dohyung == "tri_up") {
				mydohyung.type = 1;
				cin >> mydohyung.x >> mydohyung.y >> mydohyung.height >> mydohyung.brush;
				mycanvas->AddShape(mydohyung);
			}
			else if (dohyung == "tri_down") {
				mydohyung.type = 2;
				cin >> mydohyung.x >> mydohyung.y >> mydohyung.height >> mydohyung.brush;
				mycanvas->AddShape(mydohyung);
			}
		}
		else if (command == "draw") {
			mycanvas->Draw(cout);
		}
		else if (command == "dump") {
			mycanvas->Dump(cout);
		}
		else if (command == "delete") {
			cin >> idx;
			mycanvas->DeleteShape(idx);
		}
	}
	
	delete mycanvas;
	return 0;
}