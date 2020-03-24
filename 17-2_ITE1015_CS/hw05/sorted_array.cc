#include "sorted_array.h"

SortedArray::SortedArray() {
}
SortedArray::~SortedArray() {
	numbers_.clear();
}
void SortedArray::AddNumber(int num) {
	numbers_.push_back(num);
}

vector<int> SortedArray::GetSortedAscending() const {
	vector<int> v = numbers_;
	int n = v.size();
	for (int i = 1; i < n; i++) {
		int tmp = v[i];
		int j = i - 1;
		while (j >= 0 && v[j+1] < v[j]) {
			swap(v[j], v[j+1]);
			j--;
		}
	}
	return v;
}
vector<int> SortedArray::GetSortedDescending() const {
	vector<int> v = numbers_;
	int n = v.size();
	for (int i = 1; i < n; i++) {
		int j = i - 1;
		while (j >= 0 && v[j+1] > v[j]) {
			swap(v[j], v[j+1]);
			j--;
		}
	}
	return v;
}
int SortedArray::GetMax() const {
	vector<int> v = GetSortedDescending();
	return v[0];
}
int SortedArray::GetMin() const {
	vector<int> v = GetSortedAscending();
	return v[0];
}

bool SortedArray::check() {
	return (!numbers_.empty());
}
