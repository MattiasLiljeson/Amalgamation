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

	vector<Entity*>* getNetScoreEntities();

	bool checkProcessing();


};