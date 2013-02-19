#include "CircularMovementSystem.h"
#include "Transform.h"
#include "CircularMovement.h"
#include "InputBackendSystem.h"
#include <Control.h>
#include <RandomUtil.h>
#include "LoadMesh.h"

CircularMovementSystem::CircularMovementSystem()
	: EntitySystem( SystemType::CircularMovementSystem, 2, ComponentType::Transform,
	ComponentType::CircularMovement )
{
}

CircularMovementSystem::~CircularMovementSystem()
{
}

void CircularMovementSystem::initialize()
{
	initInstanceSphereByJohan("RockA.agl", AglVector3(40.0f, 0.0f, 100.0f),
		AglVector3(1.0f, 1.0f, 0.0f),  50.0f, 7500);

	m_ship = m_world->createEntity();
	m_ship->addComponent(new LoadMesh("Ship.agl"));
	AglVector3 position(-20.0f, 0, 50.0f);
	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), -position);
	m_ship->addComponent(new Transform(position, rotation, AglVector3::one()));
	m_world->addEntity(m_ship);
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
		AglVector3 vRot = AglVector3::rotateAroundAxis(v, k, circular->angle);
		AglVector3 position = circular->centerPosition + vRot;
		transform->setTranslation(position);
		AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), vRot);
		transform->setRotation(rotation);
	}
}

void CircularMovementSystem::initInstanceSphereByJohan( string p_meshName, AglVector3 p_origin,
	AglVector3 p_axis, float p_radius, unsigned int p_numberInstances )
{
	for(unsigned int i=0; i<p_numberInstances; i++)
	{
		Entity* entity = m_world->createEntity();
		AglVector3 randomDirection;
		RandomUtil::randomEvenlyDistributedSphere(&randomDirection.x, &randomDirection.y,
			&randomDirection.z);
		AglVector3 position = p_origin + randomDirection * p_radius;
		AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3(0.0f, 1.0f, 0.0f), p_origin-position);
		AglVector3 scale(1.0f, 1.0f, 1.0f);
		entity->addComponent(new Transform(position, rotation, scale));
		entity->addComponent(new LoadMesh(p_meshName));
		entity->addComponent(new CircularMovement(p_origin, p_axis, randomDirection * p_radius, 0.01f));
		m_world->addEntity(entity);
	}
}

void CircularMovementSystem::sysDisabled()
{
	vector<Entity*> entities = getActiveEntities();
	for(unsigned int i=0; i<entities.size(); i++)
	{
		m_world->deleteEntity(entities[i]);
	}
	m_world->deleteEntity(m_ship);
}