#include "MenuBackgroundSceneSystem.h"
#include "LoadMesh.h"
#include "Transform.h"
#include "CircularMovement.h"
#include <RandomUtil.h>

MenuBackgroundSceneSystem::MenuBackgroundSceneSystem()
	: EntitySystem(SystemType::MenuBackgroundSceneSystem)
{
}

MenuBackgroundSceneSystem::~MenuBackgroundSceneSystem()
{
}

void MenuBackgroundSceneSystem::initialize()
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

void MenuBackgroundSceneSystem::sysDisabled()
{
	for(unsigned int i=0; i<m_rocks.size(); i++)
	{
		if(m_rocks[i] != NULL)
		{
			m_world->deleteEntity(m_rocks[i]);
		}
	}
	m_world->deleteEntity(m_ship);
}

void MenuBackgroundSceneSystem::initInstanceSphereByJohan( string p_meshName, AglVector3 p_origin,
	AglVector3 p_axis, float p_radius, unsigned int p_numberInstances )
{
	m_rocks.resize(p_numberInstances);
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
		m_rocks[i] = entity;
	}
}