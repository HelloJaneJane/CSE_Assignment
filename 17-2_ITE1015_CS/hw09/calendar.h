#include <iostream>
#include <string>
using namespace std;

class Date {
public:
	Date();
	Date(int year, int month, int day);

	void NextDay(int n = 1);
	bool SetDate(int year, int month, int day);

	int year() const;
	int month() const;
	int day() const;

private:
	// ������ �Ǵ��Ͽ� �־��� ������ �ش��ϴ� ��¥ ��(365 �Ǵ� 366)�� ����.
	// 2���� 29�Ϸ� �� �ظ� �����̶�� ��
	static int GetDaysInYear(int year);
	// �ش� ��¥�� �ش� ������ ó��(1�� 1��)���� ��ĥ°������ ���.
	static int ComputeDaysFromYearStart(int year, int month, int day);

	int GetDayOfMonth(int year, int month);

	int year_, month_, day_;
};

struct InvalidDateException {
	string input_date;
	InvalidDateException(const string& str) : input_date(str) {}
};

// yyyy.mm.dd �������� �����.
// ����� �Է� ������ >> operator�� InvalidDateException�� �߻��� �� ����.
ostream& operator<<(ostream& os, const Date& c);
istream& operator>>(istream& is, Date& c);