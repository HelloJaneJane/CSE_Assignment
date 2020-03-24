#include "point2d.h"

int main() {
	vector<Point*> myPoint;

	while (true) {
		string command;
		cin >> command;

		if (command == "quit") break;

		if (command == "set") {
			string name;
			int X, Y;
			cin >> name >> X >> Y;

			Point *A = new Point(X, Y);
			A->name_ = name;
			myPoint.push_back(A);
						
		}
		else if (command == "eval") {
			string lhs, oper, rhs;
			cin >> lhs >> oper >> rhs;

			int Lnum, Rnum;
			int status = 3; //0:둘다문자, 1:왼쪽만숫자, 2:오른쪽만숫자, 3:둘다숫자	
			try { Lnum = stoi(lhs); }
			catch (invalid_argument e) { status = 2; }
			try { Rnum = stoi(rhs); }
			catch (invalid_argument e) {
				if (status == 3) status = 1;
				else if (status == 2) status = 0;
			}

			vector<Point*>::iterator it, Lit, Rit;
			bool RisOK = false;
			bool LisOK = false;
			bool RisMinus = false;
			bool LisMinus = false;
			if (status == 0) {
				if (lhs[0] == '-') {
					LisMinus = true;
					lhs.erase(0,1);
				}
				if (rhs[0] == '-') {
					RisMinus = true;
					rhs.erase(0,1);
				}

				for (it = myPoint.begin(); it != myPoint.end(); it++) {
					if ((*it)->name_ == rhs) {
						RisOK = true;
						Rit = it;
					}
					if ((*it)->name_ == lhs) {
						LisOK = true;
						Lit = it;
					}
				}
				if (RisOK == false || LisOK == false) cout << "input error" << endl;
				else {
					Point newR = **Rit;
					Point newL = **Lit;
					if (RisMinus) newR = -(**Rit);
					if (LisMinus) newL = -(**Lit);
					print(Oper(oper, newL, newR));
				}
			}
			else if (status == 1) {
				for (it = myPoint.begin(); it != myPoint.end(); it++) {
					if ((*it)->name_ == rhs) {
						RisOK = true;
						Rit = it;
					}
				}
				if (RisOK == false) cout << "input error" << endl;
				else print(Oper(oper, Point(Lnum), **Rit));
			}
			else if (status == 2) {
				for (it = myPoint.begin(); it != myPoint.end(); it++) {
					if ((*it)->name_ == lhs) {
						LisOK = true;
						Lit = it;
					}
				}
				if (LisOK == false) cout << "input error" << endl;
				else print(Oper(oper, **Lit, Point(Rnum)));
			}
			else if (status == 3) print(Oper(oper, Point(Lnum), Point(Rnum)));	
		}
	}
	return 0;
}