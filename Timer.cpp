#include "Timer.h"


Timer::Timer(std::string name)
{
	paused = false;
	mStartTime = -1;
	mStopTime = 0;

	mName = name;
}


Timer::~Timer(void)
{
}


void Timer::start()
{
	mStartTime = SDL_GetTicks();
}

int Timer::stop()
{
	mStopTime = SDL_GetTicks() - mStartTime;
	mStartTime = -1;

	return mStopTime;
}

void Timer::resume()
{
	mStartTime = SDL_GetTicks() - mStartTime; //Set the new start time

	paused = false;
}

void Timer::pause()
{
	mStartTime = SDL_GetTicks() - mStartTime; //Set the new start time

	paused = true;
}

void Timer::reset()
{
	mStartTime = 0;
}

int Timer::getMiliSeconds()
{
	if (!paused)
	{
		return SDL_GetTicks() - mStartTime;
	}
	else
	{
		return mStartTime;
	}
}