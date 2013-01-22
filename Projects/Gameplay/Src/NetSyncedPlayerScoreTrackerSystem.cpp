#include "NetSyncedPlayerScoreTrackerSystem.h"


NetSyncedPlayerScoreTrackerSystem::NetSyncedPlayerScoreTrackerSystem()
	: EntitySystem(SystemType::NetSyncedPlayerScoreTrackerSystem, 2,
	ComponentType::NetworkSynced, ComponentType::PlayerScore )
{
}

NetSyncedPlayerScoreTrackerSystem::~NetSyncedPlayerScoreTrackerSystem()
{
}

const vector<Entity*>& NetSyncedPlayerScoreTrackerSystem::getNetScoreEntities() const
{
	return getActiveEntities();
}

bool NetSyncedPlayerScoreTrackerSystem::checkProcessing()
{
	// Tells the ES framework to not even bother processing this system.
	return false;
}
