#pragma once

#include <EntitySystem.h>
class TcpClient;

// =======================================================================================
//	ExtrapolationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # ExtrapolationSystem
/// Detailed description...
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ExtrapolationSystem: public EntitySystem
{
public:
	ExtrapolationSystem( TcpClient* p_client );

	~ExtrapolationSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();
	float searchForLatestUpdateTimeStamp( const vector<Entity*>& p_entities );
private:
	TcpClient* m_client;
	float m_correctedDeltaTime;

};