#include "OrbitalMovementSystem.h"
#include "Transform.h"
#include "OrbitalMovement.h"
#include "InputBackendSystem.h"
#include <Control.h>

OrbitalMovementSystem::OrbitalMovementSystem()
	: EntitySystem( SystemType::CircularMovementSystem, 2, ComponentType::Transform,
	ComponentType::OrbitalMovement )
{
}

OrbitalMovementSystem::~OrbitalMovementSystem()
{
}

void OrbitalMovementSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		OrbitalMovement* circular = static_cast<OrbitalMovement*>(p_entities[i]->
			getComponent(ComponentType::OrbitalMovement));
		AglVector3 v = circular->vectorFromCenter;
		AglVector3 k = circular->axis;
		AglVector3::normalize(k);
		circular->angle += circular->angularVelocity * m_world->getDelta();
		AglVector3 vRot = AglVector3::rotateAroundAxis(v, k, circular->angle);
		AglVector3 position = circular->centerPosition + vRot;
		transform->setTranslation(position);
		//AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), vRot);
		//transform->setRotation(rotation);
	}
}