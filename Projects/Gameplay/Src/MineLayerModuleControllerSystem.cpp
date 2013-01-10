#include "MineLayerModuleControllerSystem.h"
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

MineLayerModuleControllerSystem::MineLayerModuleControllerSystem()
	: EntitySystem(SystemType::MinigunModuleControllerSystem, 1, ComponentType::MineLayerModule)
{
}


MineLayerModuleControllerSystem::~MineLayerModuleControllerSystem()
{
}

void MineLayerModuleControllerSystem::initialize()
{
}

void MineLayerModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (module->m_parentEntity >= 0)
		{
			MineLayerModule* mineLayer = static_cast<MineLayerModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::MineLayerModule)));

			mineLayer->m_cooldown -= dt;
			if (mineLayer->m_cooldown <= 0)
			{
				mineLayer->m_cooldown = 2;
				Transform* transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(p_entities[i],
					ComponentType::getTypeFor(ComponentType::Transform)));
				spawnMine(transform);
			}
		}
	}
}

void MineLayerModuleControllerSystem::spawnMine(Transform* p_transform)
{
	EntitySystem* tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int sphereMeshId = graphicsBackend->createMesh( "P_sphere" );

	Entity* entity = m_world->createEntity();
	Component* component = new RenderInfo( sphereMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );


	Transform* t = new Transform(p_transform->getTranslation(), p_transform->getRotation(), AglVector3(0.5f, 0.5f, 0.5f));
	entity->addComponent( ComponentType::Transform, t);
	m_world->addEntity(entity);
}