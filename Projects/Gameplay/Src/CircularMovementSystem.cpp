#include "CircularMovementSystem.h"
#include "Transform.h"
#include "CircularMovement.h"
#include "InputBackendSystem.h"
#include <Control.h>

CircularMovementSystem::CircularMovementSystem()
	: EntitySystem( SystemType::CircularMovementSystem, 2, ComponentType::Transform,
	ComponentType::CircularMovement )
{
}

CircularMovementSystem::~CircularMovementSystem()
{
}

void CircularMovementSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::InputBackendSystem))->
		getControlByEnum(InputHelper::KeyboardKeys_0)->getDelta() > 0.0)
	{
		m_world->getSystem(SystemType::DebugMovementSystem)->setEnabled( true );
		this->setEnabled( false );
	}

	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		CircularMovement* circular = static_cast<CircularMovement*>(p_entities[i]->
			getComponent(ComponentType::CircularMovement));
		circular->angle += m_world->getDelta() * circular->angularVelocity;
		AglVector3 position(
				circular->centerPosition.x + cos(circular->angle) * circular->radius,
				circular->centerPosition.y,
				circular->centerPosition.z + sin(circular->angle) * circular->radius);
		transform->setTranslation(position);
	}
}