#include "ShieldModuleControllerSystem.h"
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

ShieldModuleControllerSystem::ShieldModuleControllerSystem()
	: EntitySystem(SystemType::ShieldModuleControllerSystem, 1, ComponentType::ShieldModule)
{
}


ShieldModuleControllerSystem::~ShieldModuleControllerSystem()
{
}

void ShieldModuleControllerSystem::initialize()
{
}

void ShieldModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));
		if (module && module->m_parentEntity >= 0)
		{
			ShieldModule* shieldModule = static_cast<ShieldModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::ShieldModule)));
			
			handleShieldEntity(shieldModule, m_world->getEntity(module->m_parentEntity));
		}
	}
}
void ShieldModuleControllerSystem::handleShieldEntity(ShieldModule* p_module, Entity* p_parentEntity)
{
	Transform* parentTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_parentEntity,
		ComponentType::getTypeFor(ComponentType::Transform))); 

	if (p_module->m_shieldEntity >= 0)
	{
		//Update shield
		Entity* shield = m_world->getEntity(p_module->m_shieldEntity);
		Transform* transform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(shield,
			ComponentType::getTypeFor(ComponentType::Transform))); 
		transform->setTranslation(parentTransform->getTranslation());
		transform->setRotation(parentTransform->getRotation());

		p_module->m_shieldAge += m_world->getDelta();
		if (p_module->m_shieldAge > 2)
		{
			p_module->m_shieldEntity = -1;
			p_module->m_shieldAge = 0;
			m_world->deleteEntity(shield);
		}
	}
	else
	{
		p_module->m_cooldown -= m_world->getDelta();

		if (p_module->m_cooldown <= 0)
		{
			p_module->m_cooldown = 2;
			//Create Shield
			EntitySystem* tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
			GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
			int sphereMeshId = graphicsBackend->loadSingleMeshFromFile( "P_sphere" );

			Entity* entity = m_world->createEntity();
			Component* component = new RenderInfo( sphereMeshId );
			entity->addComponent( ComponentType::RenderInfo, component );


			Transform* t = new Transform(parentTransform->getTranslation(), AglQuaternion::identity(), AglVector3(2, 2, 2));
			entity->addComponent( ComponentType::Transform, t);
			m_world->addEntity(entity);
			p_module->m_shieldEntity = entity->getIndex();
		}
	}
}