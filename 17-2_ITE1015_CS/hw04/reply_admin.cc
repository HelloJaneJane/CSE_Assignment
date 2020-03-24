#include <iostream>
#include <string>
#include "reply_admin.h"

using namespace std;

void sort(int arr[], int n) {
	for (int i = 1; i<n; i++) {
		int tmp = arr[i];
		int j = i - 1;
		while (j >= 0 && tmp<arr[j]) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
	}
}

ReplyAdmin::ReplyAdmin() {

	/*for (int i = 0; i<NUM_OF_CHAT; i++) {
		chats.push_back("");
	}*/

	addChat("Hello, Reply Administrator!");
	addChat("I will be a good programmer.");
	addChat("This class is awesome.");
	addChat("Professor Lim is wise.");
	addChat("Two TAs are kind and helpful.");
	addChat("I think male TA looks cool.");
}

ReplyAdmin::~ReplyAdmin() {
	chats.clear();
}

void ReplyAdmin::printChat() {
	
	list<string>::iterator it;
	if (chats.empty()==false) {
		it = chats.begin();
		for (int i = 0; i < (int)chats.size(); i++) {
			cout << i << " " << *it << endl;
			it++;
		}
	}
}

bool ReplyAdmin::addChat(string _chat) {
	chats.push_back(_chat);
	if (chats.back() == "") return false;
	else return true;
}

bool ReplyAdmin::removeChat(int _index) {
	if (_index<0 || _index >= (int)chats.size()) return false;
	if (chats.empty()) return false;
	list<string>::iterator it = chats.begin();
	advance(it, _index);
	chats.erase(it);
	return true;
}

bool ReplyAdmin::removeChat(int *_indices, int _count) {
	if (chats.empty()) return false;
	sort(_indices, _count);

	bool result = false;

	list<string>::iterator it;
	for (int i = _count - 1; i >= 0; i--) {
		int num = _indices[i];
		if (num<0 || num >= (int)chats.size()) continue;

		it = chats.begin();
		advance(it, num);
		chats.erase(it);
		result = true;
	}

	return result;
}

bool ReplyAdmin::removeChat(int _start, int _end) {
	if (chats.empty()) return false;

	if (_start<0) _start = 0;
	if (_end >= (int)chats.size()) _end = (int)chats.size() - 1;
	
	bool result = false;
	list<string>::iterator it1 = chats.begin();
	advance(it1, _start);
	list<string>::iterator it2 = chats.begin();
	advance(it2, _end);

	chats.erase(it1, ++it2);
	result = true;
	
	return result;
}