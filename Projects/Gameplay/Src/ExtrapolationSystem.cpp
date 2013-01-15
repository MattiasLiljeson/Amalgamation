#include "ExtrapolationSystem.h"
#include <TcpClient.h>

// Components
#include "Extrapolate.h"
#include "Transform.h"

ExtrapolationSystem::ExtrapolationSystem( TcpClient* p_client )
	: EntitySystem( SystemType::ExtrapolationSystem, 2,
	ComponentType::Extrapolate, ComponentType::Transform )
{
	m_client = p_client;
}

ExtrapolationSystem::~ExtrapolationSystem()
{
}

void ExtrapolationSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		Extrapolate* extrapolate = NULL;
		extrapolate = static_cast<Extrapolate*>(p_entities[i]->getComponent(
			ComponentType::Extrapolate));

		Transform* transform = NULL;
		transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));

		
	}
}
