#include "ShipModulesControllerSystem.h"
#include "ShipModule.h"
#include "Control.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "NetworkSynced.h"
#include "PlayerScore.h"
#include "PhysicsController.h"
#include "ShipConnectionPointHighlights.h"
#include "OnHitScoreEffectPacket.h"
#include "OnHitEffectBufferSystem.h"
#include "Transform.h"
#include "ModuleHelper.h"
#include "SlotParticleEffectPacket.h"
#include "EditSphereUpdatePacket.h"
#include "ShipManagerSystem.h"
#include <ToString.h>
#include <DebugUtil.h>
#include "ScoreRuleHelper.h"

ShipModulesControllerSystem::ShipModulesControllerSystem(TcpServer* p_server,
														 ModuleVisualEffectBufferSystem* p_effectBuffer)
	: EntitySystem(SystemType::ShipModulesControllerSystem, 2, 
	ComponentType::TAG_Ship, ComponentType::ShipConnectionPointHighlights)
{
	m_server = p_server;
	m_effectbuffer = p_effectBuffer;
	m_editMode = false;
}
void ShipModulesControllerSystem::setEditMode(bool p_editMode)
{
	m_editMode = p_editMode;
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
			if (m_toHighlight[j].id == netSync->getNetworkOwner())
			{
				//Do highlight
				changeHighlight(p_entities[i], m_toHighlight[j].slot,
					m_toHighlight[j].status);
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

		//Check to see if modules should be dropped
		checkDrop_ApplyScoreAndDamage(p_entities[i]);
	}
}
void ShipModulesControllerSystem::checkDrop_ApplyScoreAndDamage(Entity* p_parent)
{

	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
		p_parent->getComponent(ComponentType::ConnectionPointSet) );

	if (connected)
	{
		for (unsigned int i = 0; i < connected->m_connectionPoints.size(); i++)
		{
			int e = connected->m_connectionPoints[i].cpConnectedEntity;
			if (e >= 0)
			{
				Entity* entity = m_world->getEntity(e);
				ShipModule* m = static_cast<ShipModule*>(entity->getComponent(ComponentType::ShipModule));

				Transform* moduleTransform = static_cast<Transform*>(
					entity->getComponent(ComponentType::Transform));

				ShipModule* parentM = static_cast<ShipModule*>(p_parent->getComponent(ComponentType::ShipModule));
				if (m && (!parentM || parentM->m_parentEntity != entity->getIndex())) //Could be a ship
				{
					m->applyDamage();
					if (m->m_health <= 0)
					{
						Entity* myShip=NULL;
						int me = ModuleHelper::FindParentShipClientId(m_world,m,&myShip);
						if (myShip)
						{
							PlayerScore* scoreComponent = static_cast<PlayerScore*>(myShip->getComponent(ComponentType::PlayerScore));
							// score effect
							if (moduleTransform && m)
							{
								// set a positive effect to perp, if not yourself
								int perpId = m->getLatestPerpetratorClient();
								if (perpId!=me)
								{
									auto ships = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
									Entity* perpShip = ships->findShip(perpId);
									if (perpShip)
									{
										PlayerScore* perpScoreComponent = static_cast<PlayerScore*>(perpShip->getComponent(ComponentType::PlayerScore));
										float score = ScoreRuleHelper::scoreFromHittingOpponent(m->m_value);
										// add score and send effect
										perpScoreComponent->addRelativeScore(score);
										setScoreEffect( perpId, moduleTransform, (int)score);
									}

								}
								// set negative effect to victim
								float score = ScoreRuleHelper::scoreFromLoseModuleOnEnemyHit(m->m_value);
								scoreComponent->addRelativeScore(score);
								setScoreEffect( me, moduleTransform, (int)score);
							}
						}
						drop(p_parent, i);
					}
					else
					{
						checkDrop_ApplyScoreAndDamage(entity);
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
	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
		p_parent->getComponent(ComponentType::ConnectionPointSet) );

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

	// ===========================
	//     Update module data
	// ===========================
	m->m_health = 100.0f;
	m->m_value = m->m_value * 0.5f;
	m->deactivate();
	m->m_lastShipEntityWhenAttached = -1; 


	//Change particle effects on slots
	if (m_editMode)
	{
		Entity* parentShip = p_parent;
		ShipModule* parentModule = static_cast<ShipModule*>(parentShip->getComponent(ComponentType::ShipModule));
		while (parentModule)
		{
			parentShip = m_world->getEntity(parentModule->m_parentEntity);
			parentModule = static_cast<ShipModule*>(parentShip->getComponent(ComponentType::ShipModule));
		}

		NetworkSynced* networkSynced = static_cast<NetworkSynced*>(
			toDrop->getComponent(ComponentType::NetworkSynced));
		NetworkSynced* parentNetworkSynced = static_cast<NetworkSynced*>(
			p_parent->getComponent(ComponentType::NetworkSynced));
		NetworkSynced* shipNetworkSynced = static_cast<NetworkSynced*>(
			parentShip->getComponent(ComponentType::NetworkSynced));

		//Remove particle effects from the slot
		for (unsigned int i = 0; i < toDropConnected->m_connectionPoints.size(); i++)
		{
			if (toDropConnected->m_connectionPoints[i].cpConnectedEntity < 0)
			{
				SlotParticleEffectPacket slotPacket;
				slotPacket.translationOffset = AglVector3(0, 0, 0);
				slotPacket.forwardDirection = AglVector3(0, 0, 0);
				slotPacket.slot = i;
				slotPacket.networkIdentity = networkSynced->getNetworkIdentity();
				slotPacket.active = false;
				m_server->unicastPacket(slotPacket.pack(), shipNetworkSynced->getNetworkOwner() );
			}
		}

		//Add back the parent slot
		SlotParticleEffectPacket slotPacket;
		slotPacket.translationOffset = connected->m_connectionPoints[p_slot].cpTransform.GetTranslation();
		slotPacket.forwardDirection = connected->m_connectionPoints[p_slot].cpTransform.GetForward();
		slotPacket.slot = p_slot;
		slotPacket.networkIdentity = parentNetworkSynced->getNetworkIdentity();
		slotPacket.active = true;
		m_server->unicastPacket(slotPacket.pack(), shipNetworkSynced->getNetworkOwner() );

		//Send a packet back to the client telling him how the edit sphere should be oriented
		ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
		EditSphereUpdatePacket editSphereUpdate;
		AglBoundingSphere bs = sms->findEditSphere(shipNetworkSynced->getNetworkOwner());
		editSphereUpdate.m_offset = bs.position;
		editSphereUpdate.m_radius = bs.radius;
		m_server->unicastPacket(editSphereUpdate.pack(), shipNetworkSynced->getNetworkOwner());
	}
}
void ShipModulesControllerSystem::addHighlightEvent(int p_slot, int p_id, int p_status)
{
	HighlightEvent e = {p_id, p_slot,p_status};
	m_toHighlight.push_back(e);
}
void ShipModulesControllerSystem::changeHighlight(Entity* p_entity, int p_new, 
												  int p_status)
{
	// Changed by Jarl 07-02-2013
	// To allow for deactivation signal for highlighting
	// of all or one slot. This is done for example for edit mode.
	// This can also be used later on if the toggle way of doing activation is changed.
	// For example if several slots need to be highlighted at ones

	// Get all slots(connection points)
	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
					p_entity->getComponent(ComponentType::ConnectionPointSet) );

	ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
		p_entity->getComponent(ComponentType::ShipConnectionPointHighlights) );

	if (p_new!=-1)
	{
		for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
		{
			if (i!=p_new)
			{
				if (highlights->slotStatus[i])
				{
					// ---------------------------------
					// This is the original code which toggles
					// separate slots on/off.
					// It will deactivate a currently active slot.
					int current = connected->m_connectionPoints[i].cpConnectedEntity;
					if (current >= 0)
					{
						Entity* currEn = m_world->getEntity(current);
						ShipModule* currModule = static_cast<ShipModule*>(currEn->getComponent(ComponentType::ShipModule));
						currModule->deactivate();
					}
					//
					highlights->slotStatus[i]=false;
					// ---------------------------------
				}
			}
			else
			{
				highlights->slotStatus[i]=true;
			}
		}	
	}
	else
	{
		// if the new specified slot==-1, deactivate all
		// this id is sent for example when switching to edit mode
		// connected->m_highlighted =
		for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
		{
			// copy of above disable code
			int current = connected->m_connectionPoints[i].cpConnectedEntity;
			if (current >= 0)
			{
				Entity* currEn = m_world->getEntity(current);
				ShipModule* currModule = static_cast<ShipModule*>(currEn->getComponent(ComponentType::ShipModule));
				currModule->deactivate();
			}
			//
			highlights->slotStatus[i]=false;
		}
	}


}
void ShipModulesControllerSystem::setActivation(Entity* p_entity, bool p_value)
{
	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
		p_entity->getComponent(ComponentType::ConnectionPointSet) );

	ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
		p_entity->getComponent(ComponentType::ShipConnectionPointHighlights) );

	for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
	{
		if (highlights->slotStatus[i])
		{
			int current = connected->m_connectionPoints[i].cpConnectedEntity;
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
	}
}
void ShipModulesControllerSystem::setActivationChildren(Entity* p_entity, bool p_value)
{
	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
		p_entity->getComponent(ComponentType::ConnectionPointSet) );

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
// float ShipModulesControllerSystem::calculateScore(Entity* p_entity)
// {
// 	float score = 0;
// 
// 	ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
// 		p_entity->getComponent(ComponentType::ConnectionPointSet) );
// 
// 	ShipModule* module = static_cast<ShipModule*>(
// 		p_entity->getComponent(ComponentType::ShipModule) );
// 
// 	if (connected)
// 	{
// 		for (unsigned int i = 0; i < connected->m_connectionPoints.size(); i++)
// 		{
// 			if (connected->m_connectionPoints[i].cpConnectedEntity >= 0)
// 			{
// 				Entity* e = m_world->getEntity(connected->m_connectionPoints[i].cpConnectedEntity);
// 				if (module && e->getIndex() == module->m_parentEntity)
// 				{
// 					//Do nothing
// 				}
// 				else
// 					score += calculateScore(e);
// 			}
// 		}
// 	}
// 	if (module)
// 		score += module->m_value;
// 	return score;
// }
void ShipModulesControllerSystem::setScoreEffect( int p_networkOwner, Transform* p_moduleTransform, 
										 int p_score )
{
	OnHitScoreEffectPacket fxPacket;
	fxPacket.score = p_score;
	fxPacket.position = p_moduleTransform->getTranslation();
	fxPacket.angle = p_moduleTransform->getRotation();

	m_effectbuffer->enqueueEffect(p_networkOwner,fxPacket);
}
