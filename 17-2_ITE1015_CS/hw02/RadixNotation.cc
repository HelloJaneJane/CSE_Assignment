#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

string RadixNotation (unsigned int number, unsigned int radix){
		
		char arr[2000];
		int cnt=0;
		while (number>0){
				int a=number%radix;
				if (a<10){
						arr[cnt]=a+48;
				}
				else if (a>=10){
						arr[cnt]=a+87;
				}
				number=number/radix;
				cnt++;
		}

		string ans="";
		while (cnt>0){
				ans+=arr[cnt-1];
				cnt--;
		}

		return ans;
}

int main (int argc, char** argv){
		if (argc < 2) return -1;
		unsigned int radix;
		sscanf(argv[1], "%u", &radix);
		if (radix < 2 || radix > 36) return -1;
		for (int i = 2; i < argc; ++i){
				unsigned int number;
				sscanf(argv[i], "%u", &number);
				cout << RadixNotation(number, radix) << endl;
		}
		return 0;
}

