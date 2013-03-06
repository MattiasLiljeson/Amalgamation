#include "MenuBackgroundSceneSystem.h"
#include "LoadMesh.h"
#include "Transform.h"
#include "CircularMovement.h"
#include "AxisRotate.h"
#include <RandomUtil.h>
#include "InputActionsBackendSystem.h"
#include "LightsComponent.h"
#include "ClientStateSystem.h"
#include "GradientComponent.h"
#include "EntityFactory.h"

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

	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));
	if(stateSystem->getStateDelta(GameStates::LOBBY) == EnumGameDelta::EXITTHISFRAME)
	{
		this->setEnabled(false);
	}
	else if(stateSystem->getStateDelta(GameStates::LOBBY) == EnumGameDelta::ENTEREDTHISFRAME){
		auto* entityFactory = static_cast<EntityFactory*>(m_world->getSystem(SystemType::EntityFactory));
		
		GradientComponent* gradient = static_cast<GradientComponent*>
			(m_ship->getComponent(ComponentType::Gradient));
		gradient->m_color.layerOne = entityFactory->getPlayersFirstGradientLevel();
		gradient->m_color.layerTwo = entityFactory->getPlayersSecondGradientLevel();
	}
	else{
		m_deltaRotation = 0.0f;
		if(m_actionBackend->getStatusByAction(InputActionsBackendSystem::
			Actions_MENU_ACTIVATE_ROTATION) != 0.0)
		{
			double deltaPositive = m_actionBackend->getStatusByAction(
				InputActionsBackendSystem::Actions_MENU_RIGHT);
			double deltaNegative = m_actionBackend->getStatusByAction(
				InputActionsBackendSystem::Actions_MENU_LEFT);
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
			rotate->angularVelocity = m_deltaRotation * 5.0f - 0.1f;
		}
	}
}

void MenuBackgroundSceneSystem::initialize()
{
	m_actionBackend = static_cast<InputActionsBackendSystem*>(m_world->getSystem(
		SystemType::InputActionsBackendSystem));
}

void MenuBackgroundSceneSystem::sysEnabled()
{
	initInstanceSphereByJohan("RockA.agl", AglVector3(40.0f, 0.0f, 100.0f),
		AglVector3(1.0f, 1.0f, 0.0f),  50.0f, 50);

	m_ship = m_world->createEntity();
	m_ship->addComponent(new LoadMesh("Ship.agl"));
	AglVector3 position(-7.5f, -2.0f, 30.0f);
	AglVector3 toVector(0.0f, -0.2f, -1.0f);
	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(), toVector);
	m_ship->addComponent(new Transform(position, rotation, AglVector3::one()));
	m_ship->addComponent(new AxisRotate(AglVector3(0.0f, 1.0f, -0.2f), toVector, rotation, 0.0f));

	m_ship->addComponent(ComponentType::Gradient, new GradientComponent(
		AglVector4(47.0f/255.0f,208.0f/255.0f,172.0f/255.0f,1),
		AglVector4(47.0f/255.0f,176.0f/255.0f,208.0f/255.0f,1)));

	m_world->addEntity(m_ship);

	Entity* entity = m_world->createEntity();
	initPointLight(entity, position + AglVector3(0.0f, 0.0f, -50.0f), 200.0f);
	m_world->addEntity(entity);
	m_lights.push_back(entity);
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
	for(unsigned int i=0; i<m_lights.size(); i++)
	{
		if(m_lights[i] != NULL)
		{
			m_world->deleteEntity(m_lights[i]);
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
		entity->addComponent(new CircularMovement(p_origin, p_axis, randomDirection * p_radius, 0.042f));
		AglVector3 randomAxis;
		RandomUtil::randomEvenlyDistributedSphere(&randomAxis.x, &randomAxis.y,
			&randomAxis.z);
		entity->addComponent(new AxisRotate(randomAxis, randomDirection * p_radius,
			AglQuaternion::identity(), RandomUtil::randomInterval(0.05, 0.2f)));
		m_world->addEntity(entity);
		m_rocks[i] = entity;
	}
}

void MenuBackgroundSceneSystem::initPointLight( Entity* p_entity, AglVector3 p_position,
	float p_range )
{
	LightsComponent* lights = new LightsComponent();
	Light light;
	light.instanceData.type = LightTypes::E_LightTypes_POINT;
	light.instanceData.enabled = true;
	light.instanceData.color[0] = 1.0f;
	light.instanceData.color[1] = 1.0f;
	light.instanceData.color[2] = 1.0f;
	light.instanceData.attenuation[0] = 0.0f;
	light.instanceData.attenuation[1] = 0.0f;
	light.instanceData.attenuation[2] = 0.0001f;
	light.instanceData.range = p_range;
	light.instanceData.spotLightConeSizeAsPow = 1.0f;
	light.instanceData.lightEnergy = 2.0f;
	TransformComponents transformComp;
	transformComp.translation = p_position;
	transformComp.scale = AglVector3(p_range, p_range, p_range);
	transformComp.rotation = AglQuaternion::identity();
	light.instanceData.setWorldTransform(transformComp.toMatrix());
	light.offsetMat = transformComp.toMatrix();
	lights->addLight(light);
	p_entity->addComponent(lights);

	p_entity->addComponent(new Transform(p_position, AglQuaternion::identity(),
		AglVector3::one()));
}