#pragma once

#include <EntitySystem.h>

class TcpClient;
class HudElement;
class LibRocketBackendSystem;

// =======================================================================================
//	DisplayPlayerScoreSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Prints players' score using anttweakbar.
///        
/// # DisplayPlayerScoreSystem
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class DisplayPlayerScoreSystem: public EntitySystem
{
public:
	DisplayPlayerScoreSystem(TcpClient* p_client);

	~DisplayPlayerScoreSystem();

	void initialize();

	void process();


	// Overrides EntitySystem::inserted
	// This method is automatically called when a new entity has been added to the system.
	void inserted( Entity* p_entity );

private:
	Entity* m_playerEntity;
	TcpClient*	m_client;
};