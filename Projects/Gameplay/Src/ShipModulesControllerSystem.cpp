#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "InputBackendSystem.h"
#include "Control.h"

ShipModulesControllerSystem::ShipModulesControllerSystem()
	: EntitySystem(SystemType::ShipModulesControllerSystem, 1, ComponentType::ShipController)
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
	double keys[5];
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
			}
		}
	}
}