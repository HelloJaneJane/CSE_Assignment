// world_clock_main.cc
#include "world_clock.h"

int main() {
	WorldClock clock;
	string cmd;
	while (cmd != "quit") {
		cin >> cmd;
		try {
			if (cmd == "set") {
				cin >> clock;
				cout << clock << endl;
			}
			else if (cmd == "tick") {
				int tick = 1;
				cin >> tick;
				clock.Tick(tick);
				cout << clock << endl;
			}
		}
		catch (InvalidTimeException e) {
			cout << "Invalid time: " << e.input_time << endl;
		}
	}
	return 0;
}
