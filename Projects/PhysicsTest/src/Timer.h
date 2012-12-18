#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

//Simple timer
class Timer
{
private: 
	double	mSecondsPerCount;
	double	mDeltaTime;
	__int64 mCurrentTime;
	__int64 mPreviousTime;
	__int64 mBaseTime;
	__int64 mTimeAtStop;
	bool	mStopped;
	double  mElapsedTime;
public:
	Timer();
	void	Tick();
	float	DeltaTime();
	float	ElapsedTime();
	void	Reset();
	void	Stop();
	void	Start();
};

#endif
