#include <iostream>
using namespace std;

int max(int a, int b) {
		if (a>=b) return a;
		else return b;
}

char arr[2000][2000];

int main(){
		for (int i=0;i<2000;i++){
				for (int j=0;j<2000;j++){
						arr[i][j]='.';
				}
		}

		int r=0, c=0;
		while (true){
				int x, y;
				cin >> x >> y;
				if (x<0&&y<0) break;

				r = max(r, y+1); // r : number of row
				c = max(c, x+1); // c : number of columns

				arr[y][x]='*';

				for (int i=0;i<r;i++){
						for (int j=0;j<c;j++){
								cout << arr[i][j];
						}
						cout << endl;
				}
		}
		return 0;
}
