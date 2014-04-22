#include "Timer.h"


Timer::Timer(void)
{
	mPreviousTime = mCurrentTime = 0;
}


Timer::~Timer(void)
{
}


int Timer::getTicks()
{
	mPreviousTime = mCurrentTime;

	mCurrentTime = SDL_GetTicks();

	return mCurrentTime;
}

int Timer::getDeltaTicks()
{
	mDeltaTime = mCurrentTime - mPreviousTime;
	return mDeltaTime;
}