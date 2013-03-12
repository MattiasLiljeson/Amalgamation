#include "PreciseTimer.h"
#include <windows.h>

PreciseTimer::PreciseTimer()
{
	m_countsPerSec = 0;
	m_startTimeStamp = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSec);
	m_secsPerCount = 1.0f / (float)m_countsPerSec;
	m_running = false;
}

PreciseTimer::~PreciseTimer()
{
}

void PreciseTimer::start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTimeStamp);
	m_running = true;
}

const bool& PreciseTimer::running() const
{
	return m_running;
}

double PreciseTimer::elapsed()
{
	__int64 currentTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTimeStamp);
	double dt = (currentTimeStamp - m_startTimeStamp) * m_secsPerCount;
	return dt;
}

double PreciseTimer::stop()
{
	__int64 currentTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTimeStamp);
	double dt = (currentTimeStamp - m_startTimeStamp) * m_secsPerCount;
	m_startTimeStamp = 0;
	m_running = false;
	return dt;
}