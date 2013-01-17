#include "RocketLauncherModuleControllerSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "..\..\Input\Src\Control.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "RocketLauncherModule.h"

RocketLauncherModuleControllerSystem::RocketLauncherModuleControllerSystem()
	: EntitySystem(SystemType::RocketLauncherModuleControllerSystem, 1, ComponentType::RocketLauncherModule)
{
}


RocketLauncherModuleControllerSystem::~RocketLauncherModuleControllerSystem()
{
}

void RocketLauncherModuleControllerSystem::initialize()
{
}

void RocketLauncherModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (gun && module && module->m_parentEntity >= 0)
		{
			handleLaserSight(p_entities[i]);

			//Check fire
			gun->coolDown = max(0, gun->coolDown - dt);

			InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::InputBackendSystem));
			Control* leftBtnControl = input->getControlByEnum(InputHelper::KEY_LCTRL);
			double pressed = leftBtnControl->getStatus();
			if(pressed == 1.0)
			{
				if (gun->coolDown == 0)
				{
					spawnRocket(p_entities[i]);
				}
			}
		}
	}
}

void RocketLauncherModuleControllerSystem::handleLaserSight(Entity* p_entity)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	if (gun->laserSightEntity < 0)
	{
		//Create Ray entity
		EntitySystem* tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
		GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
		int cubeMeshId = graphicsBackend->loadSingleMeshFromFile( "P_cube" );

		Entity* entity = m_world->createEntity();
		Component* component = new RenderInfo( cubeMeshId );
		entity->addComponent( ComponentType::RenderInfo, component );


		Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection), AglVector3(0.03f, 0.03f, 20));
		entity->addComponent( ComponentType::Transform, t);
		m_world->addEntity(entity);
		gun->laserSightEntity = entity->getIndex();
	}
	else
	{
		Transform* gunTransform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(p_entity,
			ComponentType::getTypeFor(ComponentType::Transform)));

		Entity* entity = m_world->getEntity(gun->laserSightEntity);

		Transform* laserTransform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(entity,
			ComponentType::getTypeFor(ComponentType::Transform)));

		AglQuaternion rot = gunTransform->getRotation()*AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection);

		AglVector3 offset = AglVector3(0.03f, 0.03f, 20.0f);
		rot.transformVector(offset);
		laserTransform->setTranslation(gunTransform->getTranslation()+offset);
		laserTransform->setRotation(rot);
		//laserTransform->setTranslation(scale);
	}
}
void RocketLauncherModuleControllerSystem::spawnRocket(Entity* p_entity)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	gun->coolDown = 1.0f;

	//Create Bullet
	Transform* gunTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	Transform* t = new Transform(gunTransform->getTranslation(), AglQuaternion::identity(), AglVector3(0.8f, 0.8f, 0.8f));

	AglVector3 dir = gun->fireDirection;
	const AglQuaternion& rot = gunTransform->getRotation();
	rot.transformVector(dir);


	EntitySystem* tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int cubeMeshId = graphicsBackend->loadSingleMeshFromFile( "P_cube" );

	//PhysicsSystem* physics = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
	//physics->getController()


	Entity* entity = m_world->createEntity();
	Component* component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(gunTransform->getTranslation(),
		AglQuaternion::identity(),
		AglVector3(0.8f, 0.8f, 0.8f), dir * 20.0f, 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, false));

	entity->addComponent( ComponentType::Transform, t);
	m_world->addEntity(entity);
}