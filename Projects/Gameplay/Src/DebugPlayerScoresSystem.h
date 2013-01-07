#pragma once

#define WINAPI

#include <EntitySystem.h>

// =======================================================================================
//	DebugPlayerScoresSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # DebugPlayerScoresSystem
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class DebugPlayerScoresSystem: public EntitySystem
{
public:
	DebugPlayerScoresSystem();

	~DebugPlayerScoresSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );

private:
	float m_timerResetValue;
	float m_timer;

};