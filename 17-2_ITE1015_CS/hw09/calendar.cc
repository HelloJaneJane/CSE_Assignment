// calendar.cc
#include "calendar.h"

Date::Date() {
}
Date::Date(int year, int month, int day) {
	year_ = year;
	month_ = month;
	day_ = day;
}

int Date::GetDayOfMonth(int year, int month) { // 그 달의 일 수
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) return 31;
	else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
	else if (month == 2) {
		if (GetDaysInYear(year) == 365) return 28;
		else return 29;
	}
	else return 0;
}

void Date::NextDay(int n) {
	int days = ComputeDaysFromYearStart(year_, month_, day_) + n;

	while (days < 0) { days += GetDaysInYear(--year_); }
	while (days > GetDaysInYear(year_)) { days -= GetDaysInYear(year_++); }

	month_ = 1;
	while (days > GetDayOfMonth(year_, month_)) { days -= GetDayOfMonth(year_, month_++); }

	if (days == 0) {
		if (month_ == 1) { //1월0일
			year_--;
			month_ = 12;
			day_ = 31;
		}
		else day_ = GetDayOfMonth(year_, --month_);
	}
	else day_ = days;
}
bool Date::SetDate(int year, int month, int day) {
	if (month <= 0 || month >= 13) return false;

	if (day > GetDayOfMonth(year, month) || day < 0) return false;
	
	year_ = year;
	month_ = month;
	day_ = day;
	return true;
}

int Date::year() const { return year_; }
int Date::month() const { return month_; }
int Date::day() const { return day_; }
int Date::GetDaysInYear(int year) { // 윤년이면 366, 아니면 365 
	if (year % 4 == 0) {
		if (year % 400 == 0) return 366;
		else if (year % 100 != 0) return 366;
		else return 365;
	}
	else return 365;
}

// 해당 날짜가 해당 연도의 처음(1월 1일)부터 며칠째인지를 계산.
int Date::ComputeDaysFromYearStart(int year, int month, int day) {
	if (month == 1) return day;
	else if (month == 2) return day + 31;
	else {
		int n0, n1;
		if (month >= 9) {
			n1 = (month - 1) / 2;
			n0 = (month - 4) / 2;
		}
		else if (month <= 8) {
			n1 = (month - 2) / 2;
			n0 = (month - 3) / 2;
		}

		if (GetDaysInYear(year) == 365) return 59 + 31 * n1 + 30 * n0 + day;
		else return 60 + 31 * n1 + 30 * n0 + day;
	}
}

ostream& operator<<(ostream& os, const Date& c) {
	os << c.year() << "." << c.month() << "." << c.day();
	return os;
}
istream& operator>>(istream& is, Date& c) {
	string input;
	is >> input;
	string ymd = input;
	int dot = ymd.find(".");
	int y = stoi(ymd.substr(0, dot));
	ymd = ymd.substr(dot + 1);
	dot = ymd.find(".");
	int m = stoi(ymd.substr(0, dot));
	int d = stoi(ymd.substr(dot + 1));

	bool status = c.SetDate(y, m, d);

	if (!status) throw InvalidDateException(input);
	return is;
}
