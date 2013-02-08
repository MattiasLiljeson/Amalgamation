#include "NetsyncDirectMapperSystem.h"
#include "NetworkSynced.h"

NetsyncDirectMapperSystem::NetsyncDirectMapperSystem()
	: EntitySystem(SystemType::NetsyncDirectMapperSystem, 1, ComponentType::NetworkSynced)
{
}

NetsyncDirectMapperSystem::~NetsyncDirectMapperSystem()
{
}

void NetsyncDirectMapperSystem::inserted( Entity* p_entity )
{
	NetworkSynced* netSync = static_cast<NetworkSynced*>(p_entity->getComponent(
		ComponentType::NetworkSynced));
	int networkIdentity = netSync->getNetworkIdentity();
	if(networkIdentity >= (int)m_networkEntityIndices.size())
	{
		m_networkEntityIndices.resize((unsigned int)networkIdentity + 1);
	}
	m_networkEntityIndices[networkIdentity] = p_entity->getIndex();
}

void NetsyncDirectMapperSystem::removed( Entity* p_entity )
{
	NetworkSynced* netSync = static_cast<NetworkSynced*>(p_entity->getComponent(
		ComponentType::NetworkSynced));
	int networkIdentity = netSync->getNetworkIdentity();
	m_networkEntityIndices[networkIdentity] = NULL;
}

void NetsyncDirectMapperSystem::processEntities( const vector<Entity*>& p_entities )
{
	unsigned int count = p_entities.size();
}

bool NetsyncDirectMapperSystem::checkProcessing()
{
	return true;
}

Entity* NetsyncDirectMapperSystem::getEntity( int p_networkIdentity )
{
	if(p_networkIdentity < (int)m_networkEntityIndices.size())
		return m_world->getEntity(m_networkEntityIndices.at(p_networkIdentity));
	return NULL;
}
