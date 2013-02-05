#pragma once
// =======================================================================================
// PreciseTimer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A precise timer used for debugging. And also used for game play.
///        
/// # PreciseTimer
/// Detailed description...
/// Created on: 5-2-2013 
///---------------------------------------------------------------------------------------
class PreciseTimer
{
public:
	PreciseTimer();
	~PreciseTimer();

	void start();
	const bool& running() const;
	double elapsed();
	double stop();

private:
	__int64 m_startTimeStamp;
	__int64 m_countsPerSec;
	double m_secsPerCount;
	bool m_running;

};