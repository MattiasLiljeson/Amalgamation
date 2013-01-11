#include "TimerSystem.h"

TimerSystem::TimerSystem()
	: EntitySystem( SystemType::TimerSystem )
{
	/************************************************************************/
	/* Add your own intervals here!											*/
	/************************************************************************/
	m_timerIntervals[TimerIntervals::EverySecond] = 1.0f;
	m_timerIntervals[TimerIntervals::HalfSecond] = 0.5f;
	m_timerIntervals[TimerIntervals::Every16Millisecond] = 0.016f;


	for(int i=0; i<TimerIntervals::NUM_TIMERINTERVALS; i++ )
	{
		m_timers[i] = 0;
	}

}

TimerSystem::~TimerSystem()
{
}

void TimerSystem::process()
{
	for(int i=0; i<TimerIntervals::NUM_TIMERINTERVALS; i++ )
	{
		m_timers[i] += m_world->getDelta();

		checkIfIntervalReached( (TimerIntervals::Enum)i );
	}
}

bool TimerSystem::checkTimeInterval( TimerIntervals::Enum p_timerInterval )
{
	return m_timerIntervalReached[p_timerInterval];
}

void TimerSystem::checkIfIntervalReached( TimerIntervals::Enum p_timerInterval )
{
	/************************************************************************/
	/* Checks if the current Timer has reached its interval.				*/
	/************************************************************************/
	if(m_timers[p_timerInterval] >= m_timerIntervals[p_timerInterval])
	{
		m_timers[p_timerInterval] = 0;
		m_timerIntervalReached[p_timerInterval] = true;
	}
	else
	{
		m_timerIntervalReached[p_timerInterval] = false;
	}
}
