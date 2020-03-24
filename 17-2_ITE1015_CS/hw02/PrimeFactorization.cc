#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

string primeFactorization(unsigned int n){
		string a="";
		int cnt=0;
		for (int i=2;i<=n;i++){
				if (n==1)
						break;
				else{
						if (n%i!=0)
								continue;
						else {
								cnt++;
								int idx=0;
								while (n%i==0){
										idx++;
										n=n/i;
								}
								if (cnt==1){
										a+=to_string(i)+"^"+to_string(idx);
								}
								else {
										a+=" x "+to_string(i)+"^"+to_string(idx);
								}
						}
				}
		}
	return a;
}

int main(int argc, char** argv)
{
		if (argc != 2) return -1;

		unsigned int num;
		string numstr = argv[1];
		sscanf(argv[1], "%u", &num);

		if (numstr==to_string(num))
				cout << primeFactorization(num) << endl;
		
		return 0;
}
