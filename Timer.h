#ifndef TIMER_H
#define TIMER_H

#include "Definitions.h"

#include <string>

class Timer
{
public:
	Timer(std::string name);
	~Timer(void);

	int getTicks();
	int getDeltaTicks();
	int getTotalTicks();

	int mPreviousTime;
	int mCurrentTime;
	int mDeltaTime;

private:
	std::string mName;
};

#endif