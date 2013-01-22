#pragma once

#include <EntitySystem.h>
#include <vector>
using namespace std;

// =======================================================================================
//	NetSyncedPlayerScoreTrackerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Tracks entities having both a NetworkSynced component and a PlayerScore
/// component.
///        
/// # NetSyncedPlayerScoreTrackerSystem
/// Detailed description...
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class NetSyncedPlayerScoreTrackerSystem: public EntitySystem
{
public:
	NetSyncedPlayerScoreTrackerSystem();
	~NetSyncedPlayerScoreTrackerSystem();

	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );
	vector<Entity*>* getNetScoreEntities();

private:
	vector<Entity*> m_netScoreEntities;
};