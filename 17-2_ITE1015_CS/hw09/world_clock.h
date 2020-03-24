// world_clock.h
#include <iostream>
#include <string>
using namespace std;

class WorldClock {
public:
	void Tick(int seconds = 1);
	bool SetTime(int hour, int minute, int second);
	// 잘못된 값 입력시 false 리턴하고 원래 시간은 바뀌지 않음.

	int hour() const;
	int minute() const;
	int second() const;

private:
	int hour_;
	int min_;
	int sec_;
};

struct InvalidTimeException {
	string input_time;
	InvalidTimeException(const string& str) : input_time(str) {}
};

// hh:mm:ss 형식으로 입출력
ostream& operator<<(ostream& os, const WorldClock& c);

// hh:mm:ss 로 입력받음.
// 사용자 입력 오류시 >> operator는 InvalidTimeException을 발생할 수 있음.
istream& operator>>(istream& is, WorldClock& c);
