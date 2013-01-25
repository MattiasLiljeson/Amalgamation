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
	DisplayPlayerScoreSystem(LibRocketBackendSystem* p_librocketBackend, TcpClient* p_client);

	~DisplayPlayerScoreSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );



	// Overrides EntitySystem::inserted
	// This method is automatically called when a new entity has been added to the system.
	void inserted( Entity* p_entity );

private:
	HudElement* m_playerScoreMenu;
	TcpClient*	m_client;
	int			m_scoreTableDocId;
	LibRocketBackendSystem* m_librocketBackend;
};