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

            token = inputs.substr(0, pos); //token�� ��ȣ�� ���� �ϳ������
            
            argv[argc++] = token; //token�� argv�迭��ĭ��������
            
            if(pos >= inputs.length()) break; //�ٳ־����鳡
            else inputs.erase(0, pos + 1);
        }
        
        SimpleIntSet *left = NULL;
        SimpleIntSet *right = NULL;
        
        int *elements = NULL;
        int elementCount = 0;
        OperateType type = Invalid;
        
        for(int i=0; i<argc; ++i)
        {
            string arg = argv[i]; //argv�迭���ִ°͵��̹���Ȯ���Ұž�
            if(arg == "{" && elements == NULL) elements = new int[argc]; //�����ϳ����۵Ǹ� elements�迭 ����
            else if(isdigit(arg[arg.length() - 1])) elements[elementCount++] = atoi(arg.c_str()); //�����ϰ�� elements�迭���߰�

    
            else if(arg == "}")
            {
                SimpleIntSet *newSet = new SimpleIntSet(elements, elementCount); //�ٴ��element�迭��newSet����
                if(left == NULL) left = newSet; //ù��° }�� left�� newSet�ֱ�
                else if(right == NULL) right = newSet; //�ι�°�϶� right�� �ֱ�
                
                delete elements; //�ʱ�ȭ
                elements = NULL;
                elementCount = 0;

                if(type == Invalid) type = Print;
            }
            else if(arg == "+") type = Add;
            else if(arg == "-") type = Sub;
            else if(arg == "*") type = Mul;
        }
        //left�����������Ѵ�

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