#include "message_book.h"

MessageBook::MessageBook() {
}
MessageBook::~MessageBook() {
	messages_.clear();
}
void MessageBook::AddMessage(int number, const string& message) {
	messages_.insert({ number, message });
}
void MessageBook::DeleteMessage(int number) {
	messages_.erase(number);
}
vector<int> MessageBook::GetNumbers() const {
	vector<int> Mnumbers;
	map<int, string>::const_iterator it;
	for (it = messages_.begin(); it != messages_.end(); it++) {
		Mnumbers.push_back(it->first);
	}
	return Mnumbers;
}
const string& MessageBook::GetMessage(int number) const {
	map<int, string>::const_iterator it = messages_.find(number);

	if (it != messages_.end()) {
		return it->second;
	}
	else return "";
}