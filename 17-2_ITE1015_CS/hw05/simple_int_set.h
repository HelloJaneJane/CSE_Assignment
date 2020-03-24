#include <set>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

#ifndef __hw05__simple_int_set__
#define __hw05__simple_int_set__

#define MAX_SIZE 1024

class SimpleIntSet
{
private:
    set<int> mElements;
    SimpleIntSet();
    
public:
    SimpleIntSet(int *_elements, int _count);
    ~SimpleIntSet();
	set<int> Setreturn();
	
};

void printSet(const set<int>& s);

set<int> SetIntersection(const set<int>& set0, const set<int>& set1);
set<int> SetUnion(const set<int>& set0, const set<int>& set1);
set<int> SetDifference(const set<int>& set0, const set<int>& set1);


#endif
