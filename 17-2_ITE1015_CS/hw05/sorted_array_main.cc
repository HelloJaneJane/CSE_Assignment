#include "sorted_array.h"
#include <stdexcept>

int main() {

	SortedArray *SA = new SortedArray();

	while (true) {
		string inputs;
		getline(cin, inputs);

		if (inputs == "quit") break;

		bool status;

		if (inputs.find(' ') != -1) {
			string token;
			size_t pos;

			while (true) {
				pos = inputs.find(' ');
				token = inputs.substr(0, pos);

				try { SA->AddNumber(stoi(token)); }
				catch (invalid_argument e) { status = false; }

				if (pos >= inputs.length()) break;
				else inputs.erase(0, pos + 1);
			}
		}

		status = SA->check();

		if (status == true) {
			vector<int> v;
			vector<int>::iterator it;
			if (inputs == "ascend") {
				v = SA->GetSortedAscending();
				for (it = v.begin(); it != v.end(); it++){
					cout << *it << " ";
				}
				cout << endl;
			}else if (inputs == "descend") {
				v = SA->GetSortedDescending();
				for (it = v.begin(); it != v.end(); it++) {
					cout << *it << " ";
				}
				cout << endl;
			}else if (inputs == "max") {
				cout << SA->GetMax() << endl;
			}else if (inputs == "min") {
				cout << SA->GetMin() << endl;
			}
		}
	}
	delete SA;
	return 0;
}
