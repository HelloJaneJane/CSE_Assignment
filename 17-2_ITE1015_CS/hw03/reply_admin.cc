#include "reply_admin.h"

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

int ReplyAdmin::getChatCount(){
		int i;
		for (i=0; i<NUM_OF_CHAT; ++i){
				if (chats[i].empty() == true) break;
		}
		return i;
}

ReplyAdmin::ReplyAdmin(){
		chats = new string[200];
		for(int i=0;i<NUM_OF_CHAT;i++){
				chats[i]="";
		}

		addChat("Hello, Reply Administrator!");
		addChat("I will be a good programmer.");
		addChat("This class is awesome.");
		addChat("Professor Lim is wise.");
		addChat("Two TAs are kind and helpful.");
		addChat("I think male TA looks cool.");
}

ReplyAdmin::~ReplyAdmin(){
		delete[] chats;
}

void ReplyAdmin::printChat(){
		int cnt = getChatCount();
		for (int i=0; i<cnt; ++i){
				cout << i << " " << chats[i] << endl;
		}
}

bool ReplyAdmin::addChat(string _chat){
		int i = getChatCount();
		chats[i] = _chat;
		if (chats[i]=="") return false;
		else return true;
}

bool ReplyAdmin::removeChat(int _index){
		int last=getChatCount()-1;
		if (_index<0 || _index>last) return false;

		for (int i=_index;i<last;i++){
				chats[i]=chats[i+1];
		}
		chats[last].clear();
		return true;
}

bool ReplyAdmin::removeChat(int *_indices, int _count){
			sort(_indices, _count);

			bool result=false;

			for (int i=_count-1;i>=0;i--){
					int num = _indices[i];
					int cnt = getChatCount();

					if (num<0 || num>=cnt) continue;

					for (int j=num;j<cnt-1;j++){
							chats[j]=chats[j+1];
							result = true;
					}
					chats[cnt-1].clear();
			}

			return result;
}


bool ReplyAdmin::removeChat(int _start, int _end){
		if (_start<0) _start=0;
		if (_end>=getChatCount()) _end=getChatCount()-1;
		
		bool result = false;
		int n = _end-_start+1;
		int cnt = getChatCount()-1-_end;

		for(int i=_start;i<=_end;i++){
				chats[i]="";
				result = true;
		}

		int j=_start;
		while (cnt>0){
				chats[j]=chats[j+n];
				chats[j+n].clear();
				cnt--;
				j++;
		}
		return result;
}
