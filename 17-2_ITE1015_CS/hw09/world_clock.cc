// world_clock.cc
#include "world_clock.h"

void WorldClock::Tick(int seconds) {
	int h = seconds / 3600;
	seconds %= 3600;
	int m = seconds / 60;
	seconds %= 60;
	int s = seconds;

	hour_ += h;
	min_ += m;
	sec_ += s;

	if (sec_ >= 60) {
		min_++;
		sec_ -= 60;
	}
	if (min_ >= 60) {
		hour_++;
		min_ -= 60;
	}
	if (hour_ >= 24) hour_ -= 24;

	if (sec_ < 0) {
		min_--;
		sec_ += 60;
	}
	if (min_ <0) {
		hour_--;
		min_ += 60;
	}
	if (hour_ < 0) hour_ += 24;
}

bool WorldClock::SetTime(int hour, int minute, int second) {

	if (minute >= 60 || second >= 60 || hour >= 24) return false;

	hour_ = hour;
	min_ = minute;
	sec_ = second;

	return true;
}

int WorldClock::hour() const { return hour_; }
int WorldClock::minute() const { return min_; }
int WorldClock::second() const { return sec_; }

ostream& operator<<(ostream& os, const WorldClock& c) {
	os << c.hour() << ":" << c.minute() << ":" << c.second();
	return os;
}
istream& operator>>(istream& is, WorldClock& c) {
	string input;
	is >> input;
	string hms = input;
	int co = hms.find(":");
	int h = stoi(hms.substr(0, co));
	hms = hms.substr(co + 1);
	co = hms.find(":");
	int m = stoi(hms.substr(0, co));
	int s = stoi(hms.substr(co + 1));
	
	bool status = c.SetTime(h, m, s);

	if (!status) throw InvalidTimeException(input);
	return is;
}
