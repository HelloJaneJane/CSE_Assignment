// calendar_main.cc
#include "calendar.h"

int main() {
	Date myDate;
	
	string cmd;
	while (cmd != "quit") {
		cin >> cmd;

		try {
			if (cmd == "set") {
				cin >> myDate;
				cout << myDate << endl;
			}
			else if (cmd == "next_day") {
				myDate.NextDay(1);
				cout << myDate << endl;
			}
			else if (cmd == "next") {
				int n;
				cin >> n;
				myDate.NextDay(n);
				cout << myDate << endl;
			}
		}
		catch (InvalidDateException e) {
			cout << "Invalid date: " << e.input_date << endl;
		}
	}
	return 0;
}
