#include "message_book.h"
#include <stdexcept>

int main() {
	MessageBook *Mbook = new MessageBook();
	while (true) {
		string inputs;
		getline(cin, inputs);

		if (inputs == "quit") break;
		
		bool status = true;

		if (inputs == "list") {
			vector<int> Mnumbers = Mbook->GetNumbers();
			vector<int>::iterator it;
			for (it = Mnumbers.begin(); it != Mnumbers.end(); it++) {
				cout << *it << ": " << Mbook->GetMessage(*it) << endl;
			}
		}
		
		else if (inputs.find(' ') != -1) {
			size_t pos = inputs.find(' ');
			string command = inputs.substr(0, pos);
			inputs.erase(0, pos + 1);

			pos = inputs.find(' ');
			int numb;
			try { numb = stoi(inputs.substr(0, pos)); }
			catch (invalid_argument e) { status = false; }
			
			if (status == true) {
				if (command == "add") {
					inputs.erase(0, pos + 1);
					Mbook->AddMessage(numb, inputs);
				}
				else if (command == "print") {
					cout << Mbook->GetMessage(numb) << endl;
				}
				else if (command == "delete") {
					Mbook->DeleteMessage(numb);
				}
			}
		}

	}
	delete Mbook;
	return 0;
}
