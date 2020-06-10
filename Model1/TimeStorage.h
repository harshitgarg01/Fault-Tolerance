#pragma once
#include <chrono>

class TimeStorage
{
public:
	int attempts;
	chrono::time_point<struct std::chrono::steady_clock, class std::chrono::duration<__int64, struct std::ratio<1, 1000000000> > > identifierCompletionTime;
	vector <chrono::time_point<struct std::chrono::steady_clock, class std::chrono::duration<__int64, struct std::ratio<1, 1000000000> > >> attemptsStartingTime;
	vector <chrono::time_point<struct std::chrono::steady_clock, class std::chrono::duration<__int64, struct std::ratio<1, 1000000000> > >> attemptsEndingTime;
	TimeStorage()
	{
		this->attempts = 0;
	}
};