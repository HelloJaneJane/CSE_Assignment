#include <iostream>
#include <vector>
#include <string>
using namespace std;

class SortedArray {
public:
	SortedArray();
	~SortedArray();

	void AddNumber(int num);

	vector<int> GetSortedAscending() const;
	vector<int> GetSortedDescending() const;
	int GetMax() const;
	int GetMin() const;

	bool check();

private:
	vector<int> numbers_;
};
