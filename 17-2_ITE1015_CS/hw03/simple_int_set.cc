#include <iostream>
#include <stdlib.h>
#include "simple_int_set.h"
using namespace std;

void SimpleIntSet::sortElements() {
		for (int i = 1; i<= mElementCount-1; i++) {
				int tmp = mElements[i];
				int j = i - 1;
				while (j >= 0 && tmp<mElements[j]) {
						mElements[j + 1] = mElements[j];
						j--;
				}
				mElements[j + 1] = tmp;
		}
		int c = 1;
		while (c < mElementCount) {
				if (mElements[c] == mElements[c - 1]) {
						if (c < mElementCount - 1) {
								for(int i = c; i <= mElementCount-2; i++) {
										mElements[i] = mElements[i + 1];
								}
						}
						mElementCount--;
						mElements[mElementCount] = '\0';
				}
				else c++;
		}
}

SimpleIntSet::SimpleIntSet() {
		*mElements = '\0';
		mElementCount = 0;
}

SimpleIntSet::SimpleIntSet(int *_elements, int _count) {
		mElements = new int[MAX_SIZE];
		for (int i = 0; i < _count; i++) {
				mElements[i] = _elements[i];
		}
		mElementCount = _count;
		sortElements();
}

SimpleIntSet::~SimpleIntSet() {
}

int *SimpleIntSet:: elements() const {
		return mElements;
}

int SimpleIntSet::elementCount() const {
		return mElementCount;
}

SimpleIntSet* SimpleIntSet::unionSet(SimpleIntSet& _operand){
		_operand.sortElements();
		int cnt = _operand.elementCount();
		int* arr = _operand.elements();
		
		for (int i = 0; i < cnt; i++) {
				mElements[mElementCount + i] = arr[i];
		}
		mElementCount += cnt;
		sortElements();
		return this;
}

SimpleIntSet* SimpleIntSet::differenceSet(SimpleIntSet& _operand) {
		_operand.sortElements();
		int cnt = _operand.elementCount();
		int* arr = _operand.elements();
		
		for (int i = 0; i < mElementCount; i++) {
				for (int j = 0; j < cnt; j++) {
						if (mElements[i] == arr[j]) {
								for (int l = i; l <= mElementCount-2; l++) {
										mElements[l] = mElements[l + 1];
								}
								mElementCount--;
								mElements[mElementCount] = '\0';
						}
				}
		}
		return this;
}

SimpleIntSet* SimpleIntSet::intersectSet(SimpleIntSet& _operand) {
		_operand.sortElements();
		int cnt = _operand.elementCount();
		int* arr = _operand.elements();
		
		bool status;
		int i = 0;
		while (i < mElementCount) {
				for (int j = 0; j < cnt; j++) {
						if (mElements[i] != arr[j]) {
								if (j < cnt - 1) continue;
								else if (j == cnt - 1) status = false;
						}
						else if (mElements[i] == arr[j]) {
								status = true;
								break;
						}
				}
				
				if (status==false){
						if (i < mElementCount - 1) {
								for (int l = i; l <= mElementCount-2; l++) {
										mElements[l] = mElements[l + 1];
								}
						}
						mElementCount--;
						mElements[mElementCount] = '\0';
				}
				else i++;
		}
		return this;
}

void SimpleIntSet::printSet() {
		cout << "{ ";
		for (int i = 0; i < mElementCount; i++) {
				cout << mElements[i] << " ";
		}
		cout << "}" << endl;
}
