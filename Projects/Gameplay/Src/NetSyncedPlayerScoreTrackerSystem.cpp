#include "NetSyncedPlayerScoreTrackerSystem.h"


NetSyncedPlayerScoreTrackerSystem::NetSyncedPlayerScoreTrackerSystem()
	: EntitySystem(SystemType::NetSyncedPlayerScoreTrackerSystem, 2,
	ComponentType::NetworkSynced, ComponentType::PlayerScore )
{
}

NetSyncedPlayerScoreTrackerSystem::~NetSyncedPlayerScoreTrackerSystem()
{
}

void NetSyncedPlayerScoreTrackerSystem::inserted( Entity* p_entity )
{
	m_netScoreEntities.push_back(p_entity);
}

void NetSyncedPlayerScoreTrackerSystem::removed( Entity* p_entity )
{
	for(unsigned int i=0; i<m_netScoreEntities.size(); i++)
	{
		if(m_netScoreEntities[i] == p_entity)
		{
			m_netScoreEntities.erase(m_netScoreEntities.begin() + i);
		}
	}
}

vector<Entity*>* NetSyncedPlayerScoreTrackerSystem::getNetScoreEntities()
{
	return &m_netScoreEntities;
}
