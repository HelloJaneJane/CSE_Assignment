#include "binary_search.h"

void BinarySearch::sortArray() {
	for (int i = 1; i <= mArrayCount - 1; i++) {
		int tmp = mArray[i];
		int j = i - 1;
		while (j >= 0 && tmp<mArray[j]) {
			mArray[j + 1] = mArray[j];
			j--;
		}
		mArray[j + 1] = tmp;
	}
}

BinarySearch::BinarySearch() {
	*mArray = '\0';
	mArrayCount = 0;
}

BinarySearch::BinarySearch(int *_array, int _arrayCount) {
	mArray = new int[mArrayCount];
	for (int i = 0; i < _arrayCount; i++) {
		mArray[i] = _array[i];
	}
	mArrayCount = _arrayCount;

	sortArray();
}

BinarySearch::~BinarySearch() {
}

int BinarySearch::getIndex(int _element) {
	int a = 0;
	int b = mArrayCount - 1;
	int m;
	while (a <= b) {
		m = (a + b) / 2;
		if (_element < mArray[m]) b = m - 1;
		else if (_element > mArray[m]) a = m + 1;
		else return m;
	}
	return -1;
}