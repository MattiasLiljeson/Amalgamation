#include "ShipManagerSystem.h"
#include "NetworkSynced.h"

ShipManagerSystem::ShipManagerSystem() : 
	EntitySystem( SystemType::ShipManagerSystem, 1,
	ComponentType::TAG_Ship)
{

}


ShipManagerSystem::~ShipManagerSystem()
{
}

void ShipManagerSystem::initialize()
{

}

void ShipManagerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		bool added = false;
		for (unsigned int j = 0; j < m_ships.size() && !added; j++)
		{
			if (p_entities[i]->getIndex() == m_ships[j]->getIndex())
				added = true;
		}
		if (!added)
			m_ships.push_back(p_entities[i]);
	}
}
Entity* ShipManagerSystem::findShip(int p_ownerId)
{
	for (unsigned int i = 0; i < m_ships.size(); i++)
	{
		NetworkSynced* netSync = static_cast<NetworkSynced*>(m_ships[i]->getComponent(ComponentType::NetworkSynced));
		if (netSync->getNetworkOwner() == p_ownerId)
			return m_ships[i];
	}
	return NULL;
}
vector<Entity*> ShipManagerSystem::getShips()
{ 
	return m_ships; 
}
