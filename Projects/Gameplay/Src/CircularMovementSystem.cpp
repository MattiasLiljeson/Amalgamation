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
		AglVector3 v = circular->vectorFromCenter;
		AglVector3 k = circular->axis;
		AglVector3::normalize(k);
		circular->angle += circular->angularVelocity * m_world->getDelta();
		AglVector3 vRot = v * cosf(circular->angle) + AglVector3::crossProduct(k, v) * 
			sinf(circular->angle) + k * AglVector3::dotProduct(k, v) *
			(1 - cosf(circular->angle));
		AglVector3 position = circular->centerPosition + vRot;
		transform->setTranslation(position);
		AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), vRot);
		transform->setRotation(rotation);
	}
}