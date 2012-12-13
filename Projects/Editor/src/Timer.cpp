#include "Timer.h"

Timer::Timer()
{
	//Calculate the number of seconds per each count based on program performance
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;

	//Set the startvalues to 0
	mCurrentTime = 0;
	mPreviousTime = 0;
	mDeltaTime = 0;
	mBaseTime = 0;
	mStopped = false;
}
void Timer::Tick()
{
	if (!mStopped)
	{
		//Calculate the current time
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mCurrentTime = currTime;

		//Find the elapsed time since the last tick
		mDeltaTime = (mCurrentTime - mPreviousTime)*mSecondsPerCount;

		//Update the previous time for the next tick
		mPreviousTime = mCurrentTime;
		mElapsedTime += mDeltaTime;
	}
	else
	{
		mDeltaTime = 0;
	}
}
float Timer::DeltaTime()
{
	return (float)mDeltaTime;
}
float Timer::ElapsedTime()
{
	return (float)mElapsedTime;
}
void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = mPreviousTime = mCurrentTime = currTime;
}
void Timer::Stop()
{
	if( !mStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mTimeAtStop = currTime;
		mStopped  = true;
	}
}
void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if( mStopped )
	{
		mPreviousTime = startTime;
		mTimeAtStop = 0;
		mStopped  = false;
	}
}
