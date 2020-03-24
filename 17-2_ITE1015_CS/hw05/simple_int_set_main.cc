#include "simple_int_set.h"

enum EOperateType
{
    Invalid = -1,
    
    Add,
    Sub,
    Mul,
    Print,
};
typedef enum EOperateType OperateType;

int main(void)
{
    while(true)
    {
        string inputs;
        getline(cin, inputs);
        
        if(inputs== "0") break;
        
        string *argv = new string[inputs.length()]; // more than count
        string token;
        size_t pos;
        int argc = 0;
        
        while(true)
        {
            pos = inputs.find(' ');

            token = inputs.substr(0, pos); //token에 기호나 숫자 하나씩담김
            
            argv[argc++] = token; //token을 argv배열한칸씩에넣음
            
            if(pos >= inputs.length()) break; //다넣었으면끝
            else inputs.erase(0, pos + 1);
        }
        
        SimpleIntSet *left = NULL;
        SimpleIntSet *right = NULL;
        
        int *elements = NULL;
        int elementCount = 0;
        OperateType type = Invalid;
        
        for(int i=0; i<argc; ++i)
        {
            string arg = argv[i]; //argv배열에있는것들이뭔지확인할거야
            if(arg == "{" && elements == NULL) elements = new int[argc]; //집합하나시작되면 elements배열 생성
            else if(isdigit(arg[arg.length() - 1])) elements[elementCount++] = atoi(arg.c_str()); //숫자일경우 elements배열에추가

    
            else if(arg == "}")
            {
                SimpleIntSet *newSet = new SimpleIntSet(elements, elementCount); //다담긴element배열을newSet으로
                if(left == NULL) left = newSet; //첫번째 }면 left에 newSet넣기
                else if(right == NULL) right = newSet; //두번째일땐 right에 넣기
                
                delete elements; //초기화
                elements = NULL;
                elementCount = 0;

                if(type == Invalid) type = Print;
            }
            else if(arg == "+") type = Add;
            else if(arg == "-") type = Sub;
            else if(arg == "*") type = Mul;
        }
        //left를기준으로한다

		set<int> Lset = left->Setreturn();
		set<int> Rset = right->Setreturn();

		set<int> ans;
        switch(type) 
        {
			case Add:
                ans = SetUnion(Lset, Rset); break;
            case Sub:
				ans = SetDifference(Lset, Rset); break;
            case Mul:
				ans = SetIntersection(Lset, Rset); break;
            default: break;
        }
        if(type != Invalid) printSet(ans);
        
        delete left;
        delete right;
    }
    return 0;
}