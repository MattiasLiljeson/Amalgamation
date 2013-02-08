#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "Control.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "NetworkSynced.h"
#include "PlayerScore.h"
#include "PhysicsController.h"

AglMatrix ShipModulesControllerSystem::offsetTemp(Entity* p_entity, AglMatrix p_base, AglMatrix p_offset, float p_rotation)
{
	AglMatrix transform = p_base;
	ShipModule* module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));
	vector<AglMatrix> transforms;
	transforms.push_back(p_offset);
	transforms.push_back(p_base);
	while (module)
	{
		Entity* parent = m_world->getEntity(module->m_parentEntity);

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(p_entity,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		unsigned int ind = 0;
		for (unsigned int i = 1; i < cps->m_connectionPoints.size(); i++)
		{
			if (cps->m_connectionPoints[i].cpConnectedEntity == parent->getIndex())
				ind = i;
		}

		//Child
		PhysicsBody* childBody = static_cast<PhysicsBody*>(p_entity->getComponent(
			ComponentType::PhysicsBody));
		transforms.push_back(cps->m_connectionPoints[ind].cpTransform*childBody->getOffset().inverse());

		//Parent Connection points
		cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(parent,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		ind = 0;
		for (unsigned int i = 1; i < cps->m_connectionPoints.size(); i++)
		{
			if (cps->m_connectionPoints[i].cpConnectedEntity == p_entity->getIndex())
				ind = i;
		}
		//Parent
		PhysicsBody* parentBody = static_cast<PhysicsBody*>(parent->getComponent(
			ComponentType::PhysicsBody));
		transforms.push_back(cps->m_connectionPoints[ind].cpTransform*parentBody->getOffset().inverse());

		module = static_cast<ShipModule*>(parent->getComponent(ComponentType::ShipModule));
		p_entity = parent;
	}

	AglMatrix finalTransform = AglMatrix::identityMatrix();
	AglMatrix final = AglMatrix::identityMatrix();

	bool first = true;
	while (transforms.size() > 0)
	{
		//Parent transform
		AglMatrix transform = transforms.back();

		//Child Transform
		AglMatrix childTransform = transforms[transforms.size()-2];
		AglQuaternion rot = AglQuaternion::rotateToFrom(childTransform.GetForward(), -transform.GetForward());

		if (first)//transforms.size() == 2)
		{
			//Rotate around connection axis
			AglQuaternion rot2 = AglQuaternion::constructFromAxisAndAngle(transform.GetForward(), p_rotation);
			rot = rot2*rot;
			first = false;
		}

		finalTransform = AglMatrix::createRotationMatrix(rot);

		AglVector3 childTrans = childTransform.GetTranslation();
		rot.transformVector(childTrans);

		//Negate to get correct
		finalTransform.SetTranslation(transform.GetTranslation() - childTrans);
		transforms.pop_back();
		transforms.pop_back();

		final = finalTransform*final;
	}
	return final;
}

ShipModulesControllerSystem::ShipModulesControllerSystem()
	: EntitySystem(SystemType::ShipModulesControllerSystem, 1, ComponentType::TAG_Ship)
{
	mRot = 0;
}


ShipModulesControllerSystem::~ShipModulesControllerSystem()
{
}

void ShipModulesControllerSystem::initialize()
{
}

void ShipModulesControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	mRot += m_world->getDelta();
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

					//Do some hardcoded rotation shit - WORKS!
					if (m)
					{
						/*PhysicsBody* targetBody = static_cast<PhysicsBody*>(p_parent->getComponent(ComponentType::PhysicsBody));

						ConnectionPointSet* conPoints =
							static_cast<ConnectionPointSet*>(
							m_world->getComponentManager()->getComponent(entity,
							ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

						int sel = 0;
						for (unsigned int i = 0; i < conPoints->m_connectionPoints.size(); i++)
						{
							if (conPoints->m_connectionPoints[i].cpConnectedEntity == p_parent->getIndex())
							{
								sel = i;
								break;
							}
						}
						PhysicsBody* moduleBody = static_cast<PhysicsBody*>(entity->getComponent(ComponentType::PhysicsBody));

						AglMatrix transform = offsetTemp(p_parent, connected->m_connectionPoints[i].cpTransform*targetBody->getOffset().inverse(), 
							conPoints->m_connectionPoints[sel].cpTransform*moduleBody->getOffset().inverse(), mRot);

						PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
						Body* body = ps->getController()->getBody(moduleBody->m_id);
						body->setTransform(transform);*/
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

/*PhysicsBody* b = static_cast<PhysicsBody*>(p_parent->getComponent(ComponentType::PhysicsBody));
PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
Body* body = ps->getController()->getBody(b->m_id);

AglMatrix world = body->GetWorld();
world = b->getOffset().inverse()*world;
//world *= connected->m_connectionPoints[i].cpTransform;
world = connected->m_connectionPoints[i].cpTransform*b->getOffset().inverse();

PhysicsBody* bc = static_cast<PhysicsBody*>(entity->getComponent(ComponentType::PhysicsBody));
Body* bodyc = ps->getController()->getBody(bc->m_id);
bodyc->AddAngularImpulse(world.GetForward() * m_world->getDelta(), false);*/