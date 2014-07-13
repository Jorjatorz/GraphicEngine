#ifndef TIMER_H
#define TIMER_H

#include "Definitions.h"

#include <string>

class Timer
{
public:
	Timer(std::string name);
	~Timer(void);

	int getMiliSeconds();

	void start();
	int stop();
	void resume();
	void pause();
	void reset();


	int mStartTime;
	int mStopTime;
	int mToltalTime;

private:
	std::string mName;

	bool paused;

};

#endif