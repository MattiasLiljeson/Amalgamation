#include "MenuBackgroundSceneSystem.h"
#include "LoadMesh.h"
#include "Transform.h"
#include "CircularMovement.h"
#include "AxisRotate.h"
#include <RandomUtil.h>
#include "InputBackendSystem.h"

MenuBackgroundSceneSystem::MenuBackgroundSceneSystem()
	: EntitySystem(SystemType::MenuBackgroundSceneSystem)
{
	m_deltaRotation = 0.0f;
}

MenuBackgroundSceneSystem::~MenuBackgroundSceneSystem()
{
}

void MenuBackgroundSceneSystem::process()
{
	m_deltaRotation = 0.0f;
	if(m_inputBackend->getStatusByEnum(InputHelper::MouseButtons_RIGHT) > 0.0)
	{
		double deltaPositive = m_inputBackend->getDeltaByEnum(InputHelper::MouseAxes_X_POSITIVE);
		double deltaNegative = m_inputBackend->getDeltaByEnum(InputHelper::MouseAxes_X_NEGATIVE);
		if(deltaPositive > 0.0)
		{
			m_deltaRotation -= (float)deltaPositive;
		}
		if(deltaNegative > 0.0)
		{
			m_deltaRotation += (float)deltaNegative;
		}
	}
	AxisRotate* rotate = static_cast<AxisRotate*>(m_ship->getComponent(ComponentType::AxisRotate));
	if(rotate != NULL)
	{
		rotate->angularVelocity = m_deltaRotation * 10.0f;
	}
}

void MenuBackgroundSceneSystem::initialize()
{
	m_inputBackend = static_cast<InputBackendSystem*>(m_world->getSystem(
		SystemType::InputBackendSystem));
}

void MenuBackgroundSceneSystem::sysEnabled()
{
	initInstanceSphereByJohan("RockA.agl", AglVector3(40.0f, 0.0f, 100.0f),
		AglVector3(1.0f, 1.0f, 0.0f),  50.0f, 5000);

	m_ship = m_world->createEntity();
	m_ship->addComponent(new LoadMesh("Ship.agl"));
	AglVector3 position(-20.0f, 0, 50.0f);
	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), AglVector3::backward());
	m_ship->addComponent(new Transform(position, rotation, AglVector3::one()));
	m_ship->addComponent(new AxisRotate(AglVector3::up(), AglVector3::backward(), rotation, 0.0f));
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