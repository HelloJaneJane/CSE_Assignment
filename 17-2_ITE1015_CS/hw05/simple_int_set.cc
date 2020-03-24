#include "simple_int_set.h"

SimpleIntSet::SimpleIntSet() {//�Է������������ǻ�����
	mElements.clear();
}

SimpleIntSet::SimpleIntSet(int *_elements, int _count) { //������Է��������ǻ�����
	for (int i = 0; i < _count; i++) {
		mElements.insert(_elements[i]);
	}
}

SimpleIntSet::~SimpleIntSet() { //�Ҹ��ڴ°ǵ����ʴ´� ���������εδ°Ծ����ϴ�
}

set<int> SimpleIntSet::Setreturn() {
	return mElements;
}

set<int> SetIntersection(const set<int>& set0, const set<int>& set1) {
	
	set<int>::iterator it0;
	set<int>::iterator it1;

	set<int> interSet;

	for (it0 = set0.begin(); it0 != set0.end(); it0++) {
		for (it1 = set1.begin(); it1 != set1.end(); it1++) {
			if (*it0 == *it1) {
				interSet.insert(*it0);
			}
		}
	}

	return interSet;
}

set<int> SetUnion(const set<int>& set0, const set<int>& set1) {
	set<int> unionSet = set0;
	set<int>::iterator it = set1.begin();
	while (it != set1.end()){
		unionSet.insert(*it);
		it++;
	}
	
	return unionSet;
}

set<int> SetDifference(const set<int>& set0, const set<int>& set1) {
	
	set<int>::iterator it0;
	set<int>::iterator it1;

	set<int> differSet = set0;

	for (it0 = set0.begin(); it0 != set0.end(); it0++) {
		for (it1 = set1.begin(); it1 != set1.end(); it1++) {
			if (*it0 == *it1) {
				differSet.erase(*it0);
			}
		}
	}

	return differSet;
}

void printSet(const set<int>& s) {
	cout << "{ ";
	set<int>::iterator it;
	for (it = s.begin(); it!=s.end(); it++) {
		cout << *it << " ";
	}
	cout << "}" << endl;
}


