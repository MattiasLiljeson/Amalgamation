#include "DebugMovementSystem.h"
#include "DebugMove.h"
#include "Transform.h"
#include <cmath>

DebugMovementSystem::DebugMovementSystem()
	: EntitySystem( SystemType::DebugMovementSystem, 2, ComponentType::Transform,
	ComponentType::DebugMove )
{
}

DebugMovementSystem::~DebugMovementSystem()
{
}

void DebugMovementSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		DebugMove* debugMove = static_cast<DebugMove*>(p_entities[i]->
			getComponent(ComponentType::DebugMove));
		AglVector3 translation = transform->getTranslation();
		translation += debugMove->direction * m_world->getDelta();
		transform->setTranslation(translation);
	}
}