#include <iostream>
#include <string>
#include "reply_admin.h"
using namespace std;

bool replyOperation(ReplyAdmin *_replyAdmin)
{
	string inputs;
	getline(cin, inputs);

	if (inputs == "#quit") return false;
	else if (inputs.find("#remove") != string::npos)
	{
		inputs.erase(0, 8);

		int start = -1;
		int end = -1;
		int number = -1;

		int* numbers = new int[NUM_OF_CHAT];
		int numbersCount = 0;

		bool isRemoveValid = false;

		for (int i = 0; i<(int)inputs.size(); ++i)
		{
			char c = inputs[i];

			if (isdigit(c))
			{
				int num = (int)c - (int)'0';
				if (number < 0) number = num;
				else number = number * 10 + num;
			}
			else
			{
				if (c == ',') numbers[numbersCount++] = number;
				else if (c == '-') start = number;
				number = -1;
			}
		}

		if (start >= 0) end = number; // - 경우일때 마지막숫자

		if (end >= start && start >= 0) isRemoveValid = _replyAdmin->removeChat(start, end); // - 경우 지우기
		else if (numbersCount > 0) // , 경우 지우기
		{
			numbers[numbersCount++] = number; // , 경우 final number
			isRemoveValid = _replyAdmin->removeChat(numbers, numbersCount);
		}
		else if (number >= 0) isRemoveValid = _replyAdmin->removeChat(number); // 하나짜리경우지우기

		if (isRemoveValid == true) _replyAdmin->printChat(); //지워졌으면프린트한다

		delete[] numbers;
	}
	else if (_replyAdmin->addChat(inputs)) _replyAdmin->printChat();

	return true;
}

int main(void)
{
	ReplyAdmin *replyAdmin = new ReplyAdmin();

	while (replyOperation(replyAdmin)) { /* Nothing else to do. */ }

	delete replyAdmin;
	return 0;
}