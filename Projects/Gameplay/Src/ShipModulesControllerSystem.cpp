#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "NetworkSynced.h"

ShipModulesControllerSystem::ShipModulesControllerSystem()
	: EntitySystem(SystemType::ShipModulesControllerSystem, 1, ComponentType::TAG_Ship)
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
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		NetworkSynced* netSync = static_cast<NetworkSynced*>(p_entities[i]->getComponent(ComponentType::NetworkSynced));
		
		for (unsigned int j = 0; j < m_toHighlight.size(); j++)
		{
			if (m_toHighlight[j].first == netSync->getNetworkOwner())
			{
				//Do highlight
				changeHighlight(p_entities[i], m_toHighlight[j].second);
				m_toHighlight[j] = m_toHighlight.back();
				m_toHighlight.pop_back();
				j--;
			}
		}
		for (unsigned int j = 0; j < m_toActivate.size(); j++)
		{
			if (m_toActivate[j] == netSync->getNetworkOwner())
			{
				//Do Activate
				setActivation(p_entities[i], true);
				m_toActivate[j] = m_toActivate.back();
				m_toActivate.pop_back();
				j--;
			}
		}
		for (unsigned int j = 0; j < m_toDeactivate.size(); j++)
		{
			if (m_toDeactivate[j] == netSync->getNetworkOwner())
			{
				//Do Deactivate
				setActivation(p_entities[i], false);
				m_toDeactivate[j] = m_toDeactivate.back();
				m_toDeactivate.pop_back();
				j--;
			}
		}
	}




	/*double keys[6];
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
	}*/
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
void ShipModulesControllerSystem::addHighlightEvent(int p_slot, int p_id)
{
	m_toHighlight.push_back(pair<int, int>(p_id, p_slot));
}
void ShipModulesControllerSystem::changeHighlight(Entity* p_entity, int p_new)
{
	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));


	int current = connected->m_connectionPoints[connected->m_highlighted].cpConnectedEntity;
	if (current >= 0)
	{
		Entity* currEn = m_world->getEntity(current);
		ShipModule* currModule = static_cast<ShipModule*>(currEn->getComponent(ComponentType::ShipModule));
		currModule->m_active = false;
	}

	connected->m_highlighted = p_new;
}
void ShipModulesControllerSystem::setActivation(Entity* p_entity, bool p_value)
{
	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));


	int current = connected->m_connectionPoints[connected->m_highlighted].cpConnectedEntity;
	if (current >= 0)
	{
		Entity* currEn = m_world->getEntity(current);
		ShipModule* currModule = static_cast<ShipModule*>(currEn->getComponent(ComponentType::ShipModule));
		currModule->m_active = p_value;
	}
}
void ShipModulesControllerSystem::addActivateEvent(int p_index)
{
	m_toActivate.push_back(p_index);
}
void ShipModulesControllerSystem::addDeactivateEvent(int p_index)
{
	m_toDeactivate.push_back(p_index);
}