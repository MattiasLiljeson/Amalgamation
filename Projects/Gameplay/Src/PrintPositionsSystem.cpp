#include "PrintPositionsSystem.h"

PrintPositionsSystem::PrintPositionsSystem()
	: EntitySystem( SystemType::PrintPositionsSystem, 1, ComponentType::Transform )
{

}

PrintPositionsSystem::~PrintPositionsSystem()
{

}

void PrintPositionsSystem::begin()
{
	cout << "Positional entities:\n";
}

void PrintPositionsSystem::end()
{
	cout << "\n";
}

void PrintPositionsSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		Transform* transform =
			static_cast<Transform*>(
				m_world->getComponentManager()->getComponent( p_entities[i],
				ComponentType::getTypeFor( ComponentType::Transform ) ) );

		//cout << "Position(" << transform->getTranslation().x << ", " <<
		//	transform->getTranslation().y << ", " <<
		//	transform->getTranslation().z <<").\n";
	}
}

void PrintPositionsSystem::initialize()
{
}
