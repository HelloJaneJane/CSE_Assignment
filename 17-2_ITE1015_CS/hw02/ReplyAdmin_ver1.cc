#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdexcept>
using namespace std;

const int NUM_OF_CHAT = 200;

void sort(int arr[],int n){
		for (int i=1;i<n;i++){
				int tmp=arr[i];
				int j=i-1;
				while (j>=0 && tmp<arr[j]){
						arr[j+1]=arr[j];
						j--;
				}
				arr[j+1]=tmp;
		}
}

int getChatCount(string *_chatList){
		int i;
		for (i=0; i<NUM_OF_CHAT; ++i){
				string s = _chatList[i];
				if (s.empty() == true) break;
		}
		return i;
}

void printChat(string *_chatList){
		int count = getChatCount(_chatList);
		for (int i=0; i<count; ++i){
				cout << i << " " << _chatList[i] << endl;
		}
}

bool addChat(string *_chatList, string _chat){
		int i = getChatCount(_chatList);
		_chatList[i] = _chat;
		return true;
}
bool removeChat(string *_chatList, int _index){
		int last=getChatCount(_chatList)-1;
		for (int i=_index;i<last;i++){
				_chatList[i]=_chatList[i+1];
		}
		_chatList[last]="";
		return true;
}

int main(void){
		string* chats = new string[NUM_OF_CHAT];

		addChat(chats, "Hello, Reply Administrator!");
		addChat(chats, "I will be a good programmer.");
		addChat(chats, "This class is awesome.");
		addChat(chats, "Professor Lim is wise.");
		addChat(chats, "Two TAs are kind and helpful.");
		addChat(chats, "I think male TA looks cool.");

		while(true){
				string command;
				getline(cin, command);

				bool result;

				if (command=="#quit")
						break;
				else if (command.find("#remove")==0){
						string idxstr = command.substr(8);
						stringstream idxstrm(idxstr);

						bool status=true;

						int chatnum = getChatCount(chats);
						int idxnum = idxstr.length();

						int baridx;
						int tmp=0;

						if (idxstr.find('-')!=-1&&idxstr.find(',')!=-1){
								result=false;
						}

						else {
								if (idxstr.find('-')!=-1){
										baridx = idxstr.find('-');
										tmp=2;
								}
								else if(idxstr.find(',')!=-1){
										tmp=3;
								}
								else tmp=1;
						}

						if (tmp==1){
								int indx;
								try{
									indx = stoi(idxstr);
								}
								catch (invalid_argument e){
										status = false;
								}

								if (status==true&& indx>=0 && indx<chatnum)
										result = removeChat(chats, indx);
						}

						else if (tmp==2){
								string idx1="";
								string idx2="";
								for (int i=0;i<idxnum;i++){
										if (i<baridx){
												idx1 += idxstr.at(i);
										}
										else if (i>baridx){
												idx2 += idxstr.at(i);
										}
								}
								int indx1;
								int indx2;
								try{
									indx1 = stoi(idx1);
									indx2 = stoi(idx2);
								}
								catch (invalid_argument e){
										status = false;
								}

								if (status==true){
										if (indx1<0) indx1=0;
										if (indx2>=chatnum) indx2=chatnum-1;

										for (int i=indx2; i>=indx1; i--){
												result = removeChat(chats, i);
										}
								}
						}
						
						else if (tmp==3){
								int idxs[200];
								string idx;
								int cnt=0;
								while (getline(idxstrm, idx, ',')){
										try{
												idxs[cnt] = stoi(idx);
										}
										catch (invalid_argument e){
												result=false;
												status=false;
										}
										cnt++;
								}
								if (status==true) {
										sort(idxs,cnt);
										for(int i=cnt-1;i>=0;i--){
												if (idxs[i]>=0&&idxs[i]<chatnum){
												result=removeChat(chats,idxs[i]);
											}
										}
								}
						}
						
						if (result==true){
								printChat(chats);
						}
				}

				else {
						if (command.at(0)=='#'){
								continue;
						}
						else {
								if(addChat(chats, command)) printChat(chats);
						}
				}
		}

		delete[] chats;

		return 0;
}
