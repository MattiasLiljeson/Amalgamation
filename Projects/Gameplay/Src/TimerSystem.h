#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      TimerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system that updates different timer intervals such as "each second",
/// "twice a second" and "each frame" etc.
///        
/// # TimerSystem
/// The system is used by other systems that need to update certain logic with a given
/// interval.
/// Created on: 11-1-2013 
///---------------------------------------------------------------------------------------

struct TimerIntervals
{
	enum Enum
	{
		EveryFourSeconds,
		EveryTwoSeconds,
		EverySecond,
		HalfSecond,
		Every64Millisecond,
		Every32Millisecond,
		Every16Millisecond,
		Every8Millisecond,
		NUM_TIMERINTERVALS,
	};
};

class TimerSystem: public EntitySystem
{
public:
	TimerSystem();
	~TimerSystem();

	void process();

	bool checkTimeInterval( TimerIntervals::Enum p_timerInterval );

private:
	void checkIfIntervalReached( TimerIntervals::Enum p_timerInterval );

private:
	float m_timers[TimerIntervals::NUM_TIMERINTERVALS];
	float m_timerIntervals[TimerIntervals::NUM_TIMERINTERVALS];
	bool m_timerIntervalReached[TimerIntervals::NUM_TIMERINTERVALS];
	

};