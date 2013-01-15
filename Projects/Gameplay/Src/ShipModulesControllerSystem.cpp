#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"

ShipModulesControllerSystem::ShipModulesControllerSystem()
	: EntitySystem(SystemType::ShipModulesControllerSystem, 1, ComponentType::ShipFlyController)
{
}


ShipModulesControllerSystem::~ShipModulesControllerSystem()
{
}

void ShipModulesControllerSystem::initialize()
{
}

void ShipModulesControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	double keys[6];
	InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::InputBackendSystem));
	Control* ctrl = input->getControlByEnum(InputHelper::KEY_T);
	keys[0] = ctrl->getDelta();
	ctrl = input->getControlByEnum(InputHelper::KEY_Y);
	keys[1] = ctrl->getDelta();
	ctrl = input->getControlByEnum(InputHelper::KEY_U);
	keys[2] = ctrl->getDelta();
	ctrl = input->getControlByEnum(InputHelper::KEY_I);
	keys[3] = ctrl->getDelta();
	ctrl = input->getControlByEnum(InputHelper::KEY_SPACE);
	keys[4] = ctrl->getDelta();
	ctrl = input->getControlByEnum(InputHelper::KEY_LCTRL);
	keys[5] = ctrl->getDelta();

	int toHighlight = -1;
	for (unsigned int i = 0; i < 4; i++)
		if (keys[i] > 0)
		{
			toHighlight = i;
			break;
		}



	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ConnectionPointSet* connected =
			static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		for (unsigned int j = 0; j < connected->m_connectionPoints.size(); j++)
		{
			int id = connected->m_connectionPoints[j].cpConnectedEntity;
			if (id >= 0)
			{
				Entity* child = m_world->getEntity(id);
				ShipModule* module = static_cast<ShipModule*>(child->getComponent(ComponentType::ShipModule));
				if (toHighlight >= 0)
					module->m_highlighted = toHighlight==j;

				if (module->m_highlighted && keys[5] > 0)
				{
					//Drop Module
					dropModule(p_entities[i], j);
				}
			}
		}
	}
}
void ShipModulesControllerSystem::dropModule(Entity* p_parent, unsigned int p_slot)
{

	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_parent,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	Entity* child = m_world->getEntity(connected->m_connectionPoints[p_slot].cpConnectedEntity);

	ConnectionPointSet* childConnected = static_cast<ConnectionPointSet*>(child->getComponent(ComponentType::ConnectionPointSet));
	if (childConnected)
	{
		for (unsigned int i = 0; i < childConnected->m_connectionPoints.size(); i++)
		{
			if (childConnected->m_connectionPoints[i].cpConnectedEntity >= 0)
				dropModule(child, i);
		}
	}




	connected->m_connectionPoints[p_slot].cpConnectedEntity = -1;

	ShipModule* module = static_cast<ShipModule*>(child->getComponent(ComponentType::ShipModule));

	module->m_parentEntity = -1;
}