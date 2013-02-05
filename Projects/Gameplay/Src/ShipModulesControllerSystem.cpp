#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "Control.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "NetworkSynced.h"
#include "PlayerScore.h"
#include "PhysicsController.h"

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

		PlayerScore* score = static_cast<PlayerScore*>(p_entities[i]->getComponent(ComponentType::PlayerScore));
		//Calculate score
		score->setModuleScore(calculateScore(p_entities[i]));

		//Check to see if modules should be dropped
		checkDrop(p_entities[i]);
	}
}
void ShipModulesControllerSystem::checkDrop(Entity* p_parent)
{
	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_parent,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	if (connected)
	{
		for (unsigned int i = 0; i < connected->m_connectionPoints.size(); i++)
		{
			int e = connected->m_connectionPoints[i].cpConnectedEntity;
			if (e >= 0)
			{
				Entity* entity = m_world->getEntity(e);
				ShipModule* m = static_cast<ShipModule*>(entity->getComponent(ComponentType::ShipModule));

				ShipModule* parentM = static_cast<ShipModule*>(p_parent->getComponent(ComponentType::ShipModule));
				if (m && (!parentM || parentM->m_parentEntity != entity->getIndex())) //Could be a ship
				{
					m->applyDamage();
					if (m->m_health <= 0)
					{
						drop(p_parent, i);
					}
					else
					{
						checkDrop(entity);
					}
				}
			}
		}
	}
}
void ShipModulesControllerSystem::drop(Entity* p_parent, unsigned int p_slot)
{
	if (p_slot < 0)
		return;
	//Module is dropped based on damage it sustains
	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_parent,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	Entity* toDrop = m_world->getEntity(connected->m_connectionPoints[p_slot].cpConnectedEntity);

	ShipModule* m = static_cast<ShipModule*>(toDrop->getComponent(ComponentType::ShipModule));

	ConnectionPointSet* toDropConnected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(toDrop,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	if (toDropConnected)
	{
		for (unsigned int i = 0; i < toDropConnected->m_connectionPoints.size(); i++)
		{
			int e = toDropConnected->m_connectionPoints[i].cpConnectedEntity;
			if (e >= 0 && m->m_parentEntity != e)
				drop(toDrop, i);
			else if (m->m_parentEntity == e)
			{
				toDropConnected->m_connectionPoints[i].cpConnectedEntity = -1;
			}
		}
	}

	//Perform the drop
	connected->m_connectionPoints[p_slot].cpConnectedEntity = -1;
	m->m_parentEntity = -1;
	PhysicsBody* b = static_cast<PhysicsBody*>(toDrop->getComponent(ComponentType::PhysicsBody));
	PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));

	Body* body = ps->getController()->getBody(b->m_id);
	ps->getController()->DetachBodyFromCompound((RigidBody*)body);
	b->setParentId(-1);

	//Update module data
	m->m_health = 100.0f;
	m->m_value = m->m_value * 0.5f;
	m->deactivate();
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
		currModule->deactivate();
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
		if(p_value == true)
		{
			currModule->activate();
		}
		else
		{
			currModule->deactivate();
		}
		//currModule->m_active = p_value;
		setActivationChildren(currEn, p_value);
	}
}
void ShipModulesControllerSystem::setActivationChildren(Entity* p_entity, bool p_value)
{
	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	ShipModule* module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));

	if (connected)
	{
		for (unsigned int i = 0; i < connected->m_connectionPoints.size(); i++)
		{
			if (connected->m_connectionPoints[i].cpConnectedEntity >= 0)
			{
				Entity* currEn = m_world->getEntity(connected->m_connectionPoints[i].cpConnectedEntity);
				ShipModule* currModule = static_cast<ShipModule*>(currEn->getComponent(ComponentType::ShipModule));
				

				if (currModule && (!module || module->m_parentEntity != currEn->getIndex()))
				{
					if(p_value == true)
					{
						currModule->activate();
					}
					else
					{
						currModule->deactivate();
					}
					setActivationChildren(currEn, p_value);
				}
			}
		}
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
float ShipModulesControllerSystem::calculateScore(Entity* p_entity)
{
	float score = 0;

	ConnectionPointSet* connected =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	ShipModule* module =
		static_cast<ShipModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ShipModule)));

	if (connected)
	{
		for (unsigned int i = 0; i < connected->m_connectionPoints.size(); i++)
		{
			if (connected->m_connectionPoints[i].cpConnectedEntity >= 0)
			{
				Entity* e = m_world->getEntity(connected->m_connectionPoints[i].cpConnectedEntity);
				if (module && e->getIndex() == module->m_parentEntity)
				{
					//Do nothing
				}
				else
					score += calculateScore(e);
			}
		}
	}
	if (module)
		score += module->m_value;
	return score;
}