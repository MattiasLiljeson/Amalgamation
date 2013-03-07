#include "ServerPickingSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "RayPacket.h"
#include "PhysicsBody.h"
#include "ShipManagerSystem.h"
#include "EntityCreationPacket.h"
#include "EntityType.h"
#include "NetworkSynced.h"
#include "ShipModule.h"
#include "ShipManagerSystem.h"
#include "ModuleHelper.h"
#include "ModuleStateChangePacket.h"
#include "SlotParticleEffectPacket.h"
#include "EditSphereUpdatePacket.h"
#include "ScoreRuleHelper.h"
#include "PlayerComponent.h"
#include "SelectionMarkerUpdatePacket.h"
#include "MeshOffsetTransform.h"
#include "HighlightEntityPacket.h"
#include "PlayerSystem.h"
#include "TcpServer.h"
#include "PlayerComponent.h"
#include "ModuleOnChamberStartPoint.h"

float getT(AglVector3 p_o, AglVector3 p_d, AglVector3 p_c, float p_r)
{
	//Sphere Info
	AglVector3 C = p_c;
	float radius = p_r;

	//Sphere collision algorithm
	float b = AglVector3::dotProduct(p_d, p_o - C);
	float c = AglVector3::dotProduct(p_o - C, p_o - C) - radius*radius;
	if (b*b - c >= 0)
	{
		float t1 = -b - sqrt(b*b-c);
		float t2 = -b + sqrt(b*b-c);
		return t1;
		/*if (t2 > 0 && t1 * t1 < AglVector3::lengthSquared(p_lineSegment.p2-p_lineSegment.p1))
		{
			return true;
		}*/
	}
	return -1;
}

//Projects a point on a ray
AglVector3 ServerPickingSystem::project(AglVector3 p_o, AglVector3 p_d, AglVector3 p_point)
{
	p_d.normalize();
	float t = AglVector3::dotProduct(p_d, p_point-p_o) / AglVector3::dotProduct(p_d, p_d);
	t = max(t, 0);
	return p_o + p_d*t;
}

ServerPickingSystem::ServerPickingSystem(TcpServer* p_server, 
										 ModuleVisualEffectServerBufferSystem* p_effectBuffer)
	: EntitySystem(SystemType::ServerPickingSystem, 1, ComponentType::ShipModule)
{
	m_server = p_server;
	mrota = 0;
	m_effectbuffer = p_effectBuffer;
}
ServerPickingSystem::~ServerPickingSystem(){

}


void ServerPickingSystem::initialize()
{
	m_playerSystem = static_cast<PlayerSystem*>
		(m_world->getSystem(SystemType::PlayerSystem));
}

void ServerPickingSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();
	mrota += dt;
	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (m_pickComponents[i].getLatestPick() >= 0)
		{
			project(m_world->getEntity(m_pickComponents[i].getLatestPick()), m_pickComponents[i]);
		}
		else
		{
			handleRay(m_pickComponents[i], p_entities);
		}

		//Rotate relevant modules
		if (m_pickComponents[i].getLatestPick() >= 0 && m_pickComponents[i].m_rotationDirection != 0)
		{
			Entity* moduleEntity = m_world->getEntity(m_pickComponents[i].getLatestPick());
			ShipModule*  module = static_cast<ShipModule*>(moduleEntity->getComponent(ComponentType::ShipModule));
			module->m_rotation += m_world->getDelta()*m_pickComponents[i].m_rotationDirection;
		}
		else if (m_pickComponents[i].m_latestAttached >= 0 && m_pickComponents[i].m_rotationDirection != 0)
			rotateModule(m_world->getEntity(m_pickComponents[i].m_latestAttached), m_pickComponents[i].m_rotationDirection);

		updateSelectionMarker(m_pickComponents[i]);
	}
}
void ServerPickingSystem::setRay(int p_index, AglVector3 p_o, AglVector3 p_d)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (m_pickComponents[i].m_clientIndex == p_index)
		{
			physX->getController()->SetRay(p_o, p_d, m_pickComponents[i].m_rayIndex);
			return;
		}
	}

	//Create the ray
	PickComponent pc;
	pc.m_rayIndex = physX->getController()->AddRay(p_o, p_d);
	pc.m_clientIndex = p_index;
	m_pickComponents.push_back(pc);
}
void ServerPickingSystem::setEnabled(int p_index, bool p_value)
{
	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (m_pickComponents[i].m_clientIndex == p_index)
		{
			m_pickComponents[i].m_active = p_value;
			if (!p_value)
			{
				attemptConnect(m_pickComponents[i]);
				unsetPick(m_pickComponents[i]);
			}
			return;
		}
	}
}
void ServerPickingSystem::setReleased(int p_index)
{
	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (m_pickComponents[i].m_clientIndex == p_index)
		{
			Entity* parentShip = NULL;
			ShipModule* shipModule = NULL;
			Transform* moduleTransform = NULL;

			NetworkSynced* networkComp = NULL;
			PlayerComponent* scoreComponent = NULL;

			// Get data for current module
			if (m_pickComponents[i].getLatestPick()>-1)
			{
				Entity* shipModuleEntity = m_world->getEntity(m_pickComponents[i].getLatestPick());
				shipModule = NULL;
				if (shipModuleEntity)
				{
					shipModule = static_cast<ShipModule*>(shipModuleEntity->getComponent(
							ComponentType::ShipModule));
				}
				if (shipModule)
				{
					// get parent
					if (shipModule->m_lastShipEntityWhenAttached!=-1)
						parentShip = m_world->getEntity(shipModule->m_lastShipEntityWhenAttached);

					if (parentShip){
						networkComp = static_cast<NetworkSynced*>
						(parentShip->getComponent(ComponentType::NetworkSynced));

						scoreComponent = m_playerSystem->getPlayerCompFromNetworkComp(networkComp);
					}

					// also store the current transform
					auto transformComp = shipModuleEntity->getComponent(ComponentType::Transform);
					if (transformComp) moduleTransform = static_cast<Transform*>(transformComp);
				}
			}

			//Release the picked module
			unsetPick(m_pickComponents[i]);

			m_pickComponents[i].m_active = false;
			if (shipModule)
			{
				shipModule->m_lastShipEntityWhenAttached = -1; // module is now totally detached from parent ship
				shipModule->m_health = shipModule->getMaxHealth();
			}

			// set an effect
			if (moduleTransform && parentShip && shipModule && scoreComponent)
			{
				float score = ScoreRuleHelper::scoreFromLoseModuleOnDetach(shipModule->m_value);
				scoreComponent->addRelativeScore(score);
				setScoreEffect( parentShip, moduleTransform, (int)score);
			}


			return;
		}
	}
}
void ServerPickingSystem::handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities)
{
	int previous = p_pc.m_lastHovered;
	p_pc.m_lastHovered = -1;


	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	vector<LineCollisionData> cols = physX->getPhysicsController()->LineSortedCollisions(p_pc.m_rayIndex);
	if (cols.size() > 0)
	{
		//Find closest module
		int col = -1;
		for (unsigned int i = 0; i < cols.size(); i++)
		{
			Entity* e = physX->getEntity(cols[i].bodyID);
			if (e && e->getComponent(ComponentType::ShipModule))
			{
				col = cols[i].bodyID;
				break;
			}
		}

		if (col >= 0)
		{
			for (unsigned int i = 0; i < p_entities.size(); i++)
			{
				PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(
					ComponentType::PhysicsBody));
				if (pb && pb->m_id == col)
				{
					//Found a pick

					//Verify that the pick is not already picked
					bool alreadypicked = false;
					for (unsigned int pcs = 0; pcs < m_pickComponents.size(); pcs++)
					{
						if (m_pickComponents[pcs].getLatestPick() == p_entities[i]->getIndex())
							alreadypicked = true;
					}
					if (alreadypicked)
						break;

					//Only allow picking a certain distance
					ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
					Entity* rayShip = sms->findShip(p_pc.m_clientIndex);

					Transform* t1 = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
					Transform* t2 = static_cast<Transform*>(rayShip->getComponent(ComponentType::Transform));
					if ((t1->getTranslation()-t2->getTranslation()).lengthSquared() > 1600)
						break;

					//Send a message to the client showing the highlight of the object
					if (p_entities[i]->getIndex() != previous)
					{
						HighlightEntityPacket high;
						high.target = p_entities[i]->getIndex();
						high.on = true;
						m_server->unicastPacket(high.pack(), p_pc.m_clientIndex);
					}
					p_pc.m_lastHovered = p_entities[i]->getIndex();

					//Do not pick up the module if the ray is not active
					if (!p_pc.m_active)
						break;

					p_pc.setLatestPick(p_entities[i]->getIndex());

					//Attempt a detach if the entity is already connected
					if (attemptDetach(p_pc))
					{
						AglVector3 origin;
						AglVector3 dir;
						physX->getController()->GetRay(p_pc.m_rayIndex, origin, dir);

						AglVector3 d = physX->getController()->getBody(col)->GetWorld().GetTranslation()-origin;
						p_pc.m_preferredDistance = d.length();
					}
					else
					{
						unsetPick(p_pc);
					}
					break;
				}
			}
		}
	}
	if (previous != p_pc.m_lastHovered && previous != -1)
	{
		//Unhighlight previous
		HighlightEntityPacket unhigh;
		unhigh.target = previous;
		unhigh.on = false;
		m_server->unicastPacket(unhigh.pack(), p_pc.m_clientIndex);
	}
}
void ServerPickingSystem::project(Entity* toProject, PickComponent& p_ray)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	AglVector3 origin;
	AglVector3 dir;
	physX->getController()->GetRay(p_ray.m_rayIndex, origin, dir);


	PhysicsBody* projectBody = static_cast<PhysicsBody*>(toProject->getComponent(
		ComponentType::PhysicsBody));
	Body* body = physX->getController()->getBody(projectBody->m_id);

	ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(
		SystemType::ShipManagerSystem));

	//Handle the ship
	Entity* ship = sms->findShip(p_ray.m_clientIndex);

	PhysicsBody* shipBody = static_cast<PhysicsBody*>(ship->getComponent(
		ComponentType::PhysicsBody));
	Body* physicalShipBody = physX->getController()->getBody(shipBody->m_id);

	//Ship should always be a compound body
	CompoundBody* physicalShipCompoundBody = static_cast<CompoundBody*>(physicalShipBody);

	AglBoundingSphere bs = physicalShipCompoundBody->GetBoundingSphere();

	AglVector3 vel = body->GetVelocity() * body->GetMass();

	float t = getT(origin, dir, bs.position, bs.radius);
	if (t > 0)
	{
		AglVector3 dest = origin + dir*t;
		body->AddImpulse(-vel + (dest - body->GetWorld().GetTranslation())*10 * body->GetMass());
	}
	else
	{
		AglVector3 dest = project(origin, dir, bs.position);
		AglVector3 dir = dest - bs.position;
		dir.normalize();
		dest = bs.position + dir * bs.radius;
		body->AddImpulse(-vel + (dest - body->GetWorld().GetTranslation())*10 * body->GetMass());
	}

	Transform* toProjectTransform = static_cast<Transform*>
		(toProject->getComponent(ComponentType::Transform));

	closestConnectionPoint(toProjectTransform->getTranslation(), ship, p_ray);
}
AglVector3 ServerPickingSystem::closestConnectionPoint(AglVector3 p_position, 
													   Entity* p_entity, PickComponent& p_pc)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	ConnectionPointSet* conPoints = static_cast<ConnectionPointSet*>
		(p_entity->getComponent(ComponentType::ConnectionPointSet));
	Transform* transform = static_cast<Transform*>(p_entity->getComponent(
		ComponentType::Transform));

	vector<pair<int, Entity*>> free = getFreeConnectionPoints(conPoints, p_entity);

	PhysicsBody* phyBody = static_cast<PhysicsBody*>(p_entity->getComponent(
		ComponentType::PhysicsBody));
	Body* b = physX->getController()->getBody(phyBody->m_id);
	AglVector3 parentPos = b->GetWorld().GetTranslation();

	AglVector3 closest = AglVector3(0, 0, 0);
	if (free.size() > 0)
	{
		AglVector3 pos(FLT_MAX, FLT_MAX, FLT_MAX);// = (conPoints->m_connectionPoints[free[0].first].cpTransform*transform->getMatrix()).GetTranslation();
		closest = pos;
		p_pc.m_targetEntity = free[0].second->getIndex();
		p_pc.m_targetSlot = free[0].first;
		for (unsigned int i = 0; i < free.size(); i++)
		{
			conPoints = static_cast<ConnectionPointSet*>(free[i].second->getComponent(ComponentType::ConnectionPointSet));
			transform = static_cast<Transform*>(free[i].second->getComponent(ComponentType::Transform));
			AglVector3 pos = conPoints->m_connectionPoints[free[i].first].cpTransform.GetTranslation();
			pos.transform(transform->getMatrix());
			if (AglVector3::lengthSquared(pos-p_position) < AglVector3::lengthSquared(closest-p_position) && free[i].second->getIndex() != p_pc.getLatestPick()) //Can't target itself
			{
				closest = pos;
				p_pc.m_targetEntity = free[i].second->getIndex();
				p_pc.m_targetSlot = free[i].first;
			}
		}
	}
	else
	{
		p_pc.m_targetEntity = -1;
		p_pc.m_targetSlot = -1;
	}
	return closest;
}


vector<pair<int, Entity*>> ServerPickingSystem::getFreeConnectionPoints(
	ConnectionPointSet* p_set, Entity* p_parent)
{
	vector<pair<int, Entity*>> free;
	for (unsigned int i = 0; i < p_set->m_connectionPoints.size(); i++)
	{
		if (p_set->m_connectionPoints[i].cpConnectedEntity < 0)
			free.push_back(pair<int, Entity*>(
			i, p_parent));
		else
		{
			ShipModule* parentModule = static_cast<ShipModule*>(p_parent->getComponent(ComponentType::ShipModule));
			if (!parentModule || (parentModule->m_parentEntity != p_set->m_connectionPoints[i].cpConnectedEntity))
			{
				Entity* module = m_world->getEntity(p_set->m_connectionPoints[i].cpConnectedEntity);

				ConnectionPointSet* connector =
					static_cast<ConnectionPointSet*>(
					m_world->getComponentManager()->getComponent(module,
					ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
				if (connector)
				{
					vector<pair<int, Entity*>> moreFree 
						= getFreeConnectionPoints(connector, module);
					for (unsigned int j = 0; j < moreFree.size(); j++)
						free.push_back(moreFree[j]);
				}
			}
		}
	}
	return free;
}

AglMatrix ServerPickingSystem::offsetTemp(Entity* p_entity, AglMatrix p_base, AglMatrix p_offset, float p_rotation)
{
	AglMatrix transform = p_base;
	ShipModule* module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));
	vector<AglMatrix> transforms;
	vector<AglQuaternion> rots;
	transforms.push_back(p_offset);
	transforms.push_back(p_base);
	rots.push_back(AglQuaternion::constructFromAxisAndAngle(p_base.GetForward(), p_rotation));
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

		//Child module
		module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));

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

		AglMatrix parentTrans = cps->m_connectionPoints[ind].cpTransform*parentBody->getOffset().inverse();
		transforms.push_back(parentTrans);
		rots.push_back(AglQuaternion::constructFromAxisAndAngle(parentTrans.GetForward(), module->m_rotation));
		
		module = static_cast<ShipModule*>(parent->getComponent(ComponentType::ShipModule));
		p_entity = parent;
	}

	AglMatrix finalTransform = AglMatrix::identityMatrix();
	AglMatrix final = AglMatrix::identityMatrix();

	//bool first = true;
	while (transforms.size() > 0)
	{
		//Parent transform
		AglMatrix transform = transforms.back();

		//Child Transform
		AglMatrix childTransform = transforms[transforms.size()-2];
		AglQuaternion rot = AglQuaternion::rotateToFrom(childTransform.GetForward(), -transform.GetForward());

		if (true)//transforms.size() == 2)
		{
			//Rotate around connection axis
			//AglQuaternion rot2 = AglQuaternion::constructFromAxisAndAngle(transform.GetForward(), p_rotation);
			rot = rots.back()*rot;
			rot.normalize();
			rots.pop_back();
			//first = false;
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


void ServerPickingSystem::attemptConnect(PickComponent& p_ray)
{
	if (p_ray.getLatestPick() >= 0 && p_ray.m_targetEntity >= 0)
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		//Module
		Entity* module = m_world->getEntity(p_ray.getLatestPick());
		Transform* moduleTransform = static_cast<Transform*>(module->getComponent(
			ComponentType::Transform));
		ShipModule* shipModule = static_cast<ShipModule*>(module->getComponent(
			ComponentType::ShipModule));
		PhysicsBody* moduleBody = static_cast<PhysicsBody*>(module->getComponent(
			ComponentType::PhysicsBody));

		//Find module connection point
		ConnectionPointSet* conPoints = static_cast<ConnectionPointSet*>(module->getComponent(ComponentType::ConnectionPointSet));

		//Don't allow connection if the module has no connection points
		if (!conPoints)
			return;

		int sel = -1;
		for (unsigned int i = 0; i < conPoints->m_connectionPoints.size(); i++)
		{
			if (conPoints->m_connectionPoints[i].cpConnectedEntity < 0)
			{
				sel = i;
				break;
			}
		}

		//Don't allow connection if the module doesn't have any free connection points
		if (sel < 0)
			return;

		//Target
		Entity* target = m_world->getEntity(p_ray.m_targetEntity);
		Entity* ship = target;
		Entity* firstChild = NULL;
		int shipSlot = p_ray.m_targetSlot;
		while (ship->getComponent(ComponentType::ShipModule))
		{
			ShipModule* intermediate = static_cast<ShipModule*>(ship->getComponent(
				ComponentType::ShipModule));
			firstChild = ship;
			ship = m_world->getEntity(intermediate->m_parentEntity);
		}

		if (firstChild)
		{
			ConnectionPointSet* shipcps = static_cast<ConnectionPointSet*>(
				m_world->getComponentManager()->getComponent(ship,
				ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
			for (unsigned int i = 0; i < shipcps->m_connectionPoints.size(); i++)
			{
				if (shipcps->m_connectionPoints[i].cpConnectedEntity == firstChild->getIndex())
				{
					shipSlot = i;
					break;
				}
			}
		}

		PhysicsBody* shipBody = static_cast<PhysicsBody*>(ship->getComponent(
			ComponentType::PhysicsBody));

		PhysicsBody* targetBody = static_cast<PhysicsBody*>(target->getComponent(
			ComponentType::PhysicsBody));

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(target,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		CompoundBody* comp = (CompoundBody*)physX->getController()->getBody(shipBody->m_id);
		RigidBody* r = (RigidBody*)physX->getController()->getBody(moduleBody->m_id);

		//Parent transform
		AglMatrix transform = offsetTemp(target, cps->m_connectionPoints[p_ray.m_targetSlot].cpTransform*targetBody->getOffset().inverse(), 
			conPoints->m_connectionPoints[sel].cpTransform*moduleBody->getOffset().inverse(), shipModule->m_rotation);
		//AglMatrix transform = offsetTemp(target, cps->m_connectionPoints[p_ray.m_targetSlot].cpTransform, conPoints->m_connectionPoints[sel].cpTransform);

		//transform *= shipBody->getOffset().inverse();

		physX->getController()->AttachBodyToCompound(comp, r, transform);
		
		//Set the parent connection point
		cps->m_connectionPoints[p_ray.m_targetSlot].cpConnectedEntity = module->getIndex();
		

		// set an effect
		if (shipModule->m_lastShipEntityWhenAttached == -1) // only if not attached/moved before
		{
			float score = ScoreRuleHelper::scoreFromAttachModule(shipModule->m_value, 
															   shipModule->isUnused());

			NetworkSynced* networkComp = static_cast<NetworkSynced*>(ship->getComponent(
				ComponentType::NetworkSynced));
			PlayerComponent* scoreComponent = m_playerSystem->getPlayerCompFromNetworkComp(networkComp);

			scoreComponent->addRelativeScore(score);
			setScoreEffect( ship, moduleTransform, (int)score);
		}

		// Set the shipmodule to used status!
		// Must be done after score has been set.
		shipModule->setToUsed();
		setModuleUsedStatusEffect( module );

		//Set the module connection point
		conPoints->m_connectionPoints[sel].cpConnectedEntity = target->getIndex();

		shipModule->m_parentEntity = target->getIndex();
		shipModule->m_lastShipEntityWhenAttached = ship->getIndex();

		moduleBody->setParentId(shipBody->m_id);

		p_ray.m_latestAttached = module->getIndex();

		NetworkSynced* networkSynced = static_cast<NetworkSynced*>(
			module->getComponent(ComponentType::NetworkSynced));
		NetworkSynced* parentNetworkSynced = static_cast<NetworkSynced*>(
			target->getComponent(ComponentType::NetworkSynced));
		NetworkSynced* shipNetworkSynced = static_cast<NetworkSynced*>(
			ship->getComponent(ComponentType::NetworkSynced));

		//Add particle effects to the slot
		for (unsigned int i = 0; i < conPoints->m_connectionPoints.size(); i++)
		{
			if (conPoints->m_connectionPoints[i].cpConnectedEntity < 0)
			{
				SlotParticleEffectPacket slotPacket;
				slotPacket.translationOffset = conPoints->m_connectionPoints[i].cpTransform.GetTranslation();
				slotPacket.forwardDirection = conPoints->m_connectionPoints[i].cpTransform.GetForward();
				slotPacket.slot = i;
				slotPacket.networkIdentity = networkSynced->getNetworkIdentity();
				slotPacket.active = true;
				m_server->unicastPacket(slotPacket.pack(), shipNetworkSynced->getNetworkOwner() );
			}
		}

		//Remove the parent slot
		SlotParticleEffectPacket slotPacket;
		slotPacket.translationOffset = AglVector3(0, 0, 0);
		slotPacket.forwardDirection = AglVector3(0, 0, 0);
		slotPacket.slot = p_ray.m_targetSlot;
		slotPacket.shipSlot = shipSlot;
		slotPacket.networkIdentity = parentNetworkSynced->getNetworkIdentity();
		slotPacket.moduleType = networkSynced->getNetworkType();
		slotPacket.active = false;

		//Consider adding data concerning the connected entity

		m_server->unicastPacket(slotPacket.pack(), shipNetworkSynced->getNetworkOwner() );


		//Send a packet back to the client telling him how the edit sphere should be oriented
		ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
		EditSphereUpdatePacket editSphereUpdate;
		AglBoundingSphere bs = sms->findEditSphere(shipNetworkSynced->getNetworkOwner());
		editSphereUpdate.m_offset = bs.position;
		editSphereUpdate.m_radius = bs.radius;
		m_server->unicastPacket(editSphereUpdate.pack(), shipNetworkSynced->getNetworkOwner());

		// If the module was spawned on a spawnpoint, then the module wont be on the
		// on the spawnpoint anymore. Remove that component if it exists.
		auto chamberSpawnPoint = static_cast<ModuleOnChamberStartPoint*>(
			module->getComponent(ComponentType::ModuleOnChamberSpawnPoint));
		if (chamberSpawnPoint)
		{
			module->removeComponent(ComponentType::ModuleOnChamberSpawnPoint);
			module->applyComponentChanges();
		}

		/************************************************************************/
		/* SEND TO CLIENTS!!!!  shipModule->m_parentEntity						*/
		/************************************************************************/
		ModuleStateChangePacket moduleChanged;
		moduleChanged.affectedModule = networkSynced->getNetworkIdentity();
		moduleChanged.currentParrent = shipNetworkSynced->getNetworkIdentity();

		m_server->broadcastPacket(moduleChanged.pack());
	}
}
bool ServerPickingSystem::attemptDetach(PickComponent& p_ray)
{
	if (p_ray.getLatestPick() >= 0)
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		//Module
		Entity* module = m_world->getEntity(p_ray.getLatestPick());
		Transform* moduleTransform = static_cast<Transform*>(module->getComponent(
			ComponentType::Transform));
		ShipModule* shipModule = static_cast<ShipModule*>(module->getComponent(
			ComponentType::ShipModule));

		PhysicsBody* moduleBody = static_cast<PhysicsBody*>(module->getComponent(
			ComponentType::PhysicsBody));

		//Make sure the module is not connecting other modules
		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(module,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
		if (cps)
		{
			for (unsigned int i = 0; i < cps->m_connectionPoints.size(); i++)
			{
				if (cps->m_connectionPoints[i].cpConnectedEntity >= 0 &&
					cps->m_connectionPoints[i].cpConnectedEntity != shipModule->m_parentEntity)
					return false;
			}
		}

		if (shipModule->m_parentEntity >= 0)
		{
			//Get the parent
			Entity* parent = m_world->getEntity(shipModule->m_parentEntity);

			Entity* ship = parent;
			Entity* firstChild = module;
			int shipSlot = -1;
			while (ship->getComponent(ComponentType::ShipModule))
			{
				ShipModule* intermediate = static_cast<ShipModule*>(ship->getComponent(
					ComponentType::ShipModule));
				firstChild = ship;
				ship = m_world->getEntity(intermediate->m_parentEntity);
			}

			ConnectionPointSet* shipcps = static_cast<ConnectionPointSet*>(
				m_world->getComponentManager()->getComponent(ship,
				ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
			for (unsigned int i = 0; i < shipcps->m_connectionPoints.size(); i++)
			{
				if (shipcps->m_connectionPoints[i].cpConnectedEntity == firstChild->getIndex())
				{
					shipSlot = i;
					break;
				}
			}


			//Ensure that the module is not connected to an enemy ship.
			ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
			Entity* rayShip = sms->findShip(p_ray.m_clientIndex);
			
			Entity* parentShip = parent;
			ShipModule* parentModule = static_cast<ShipModule*>(parentShip->getComponent(
				ComponentType::ShipModule));

			ModuleHelper::FindParentShip(m_world,&parentShip,parentModule);

			if (parentShip != rayShip)
				return false;


			ConnectionPointSet* cpsParent = static_cast<ConnectionPointSet*>(
				m_world->getComponentManager()->getComponent(parent,
				ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
			
			//Find parent slot
			int slot = 0;
			for (unsigned int i = 0; i < cpsParent->m_connectionPoints.size(); i++)
			{
				if (cpsParent->m_connectionPoints[i].cpConnectedEntity == module->getIndex())
				{
					slot = i;
					break;
				}
			}

			//Find Child slot
			int childslot = 0;
			for (unsigned int i = 0; i < cps->m_connectionPoints.size(); i++)
			{
				if (cps->m_connectionPoints[i].cpConnectedEntity == parent->getIndex())
				{
					childslot = i;
					break;
				}
			}

			//Detach
			cpsParent->m_connectionPoints[slot].cpConnectedEntity = -1;
			cps->m_connectionPoints[childslot].cpConnectedEntity = -1;
			shipModule->m_parentEntity = -1;
			moduleBody->setParentId(-1);

			RigidBody* body = (RigidBody*)physX->getController()->getBody(moduleBody->m_id);
			physX->getController()->DetachBodyFromCompound(body, false);

			/************************************************************************/
			/* SEND DETACH MESSAGE TO CLIENTS										*/
			/************************************************************************/

			NetworkSynced* networkSynced = static_cast<NetworkSynced*>(
				module->getComponent(ComponentType::NetworkSynced));
			NetworkSynced* parentNetworkSynced = static_cast<NetworkSynced*>(
				parent->getComponent(ComponentType::NetworkSynced));
			NetworkSynced* shipNetworkSynced = static_cast<NetworkSynced*>(
				parentShip->getComponent(ComponentType::NetworkSynced));

			//Remove particle effects from the slot
			for (unsigned int i = 0; i < cps->m_connectionPoints.size(); i++)
			{
				if (cps->m_connectionPoints[i].cpConnectedEntity < 0)
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
			slotPacket.translationOffset = cpsParent->m_connectionPoints[slot].cpTransform.GetTranslation();
			slotPacket.forwardDirection = cpsParent->m_connectionPoints[slot].cpTransform.GetForward();
			slotPacket.slot = slot;
			slotPacket.networkIdentity = parentNetworkSynced->getNetworkIdentity();
			slotPacket.active = true;
			slotPacket.moduleType = networkSynced->getNetworkType();
			slotPacket.shipSlot = shipSlot;
			m_server->unicastPacket(slotPacket.pack(), shipNetworkSynced->getNetworkOwner() );

			//Send a packet back to the client telling him how the edit sphere should be oriented
			EditSphereUpdatePacket editSphereUpdate;
			AglBoundingSphere bs = sms->findEditSphere(shipNetworkSynced->getNetworkOwner());
			editSphereUpdate.m_offset = bs.position;
			editSphereUpdate.m_radius = bs.radius;
			m_server->unicastPacket(editSphereUpdate.pack(), shipNetworkSynced->getNetworkOwner());

			/************************************************************************/
			/* SEND TO CLIENTS!!!!  shipModule->m_parentEntity						*/
			/************************************************************************/
			ModuleStateChangePacket moduleChanged;
			moduleChanged.affectedModule = networkSynced->getNetworkIdentity();
			moduleChanged.currentParrent = -1;

			m_server->broadcastPacket(moduleChanged.pack());
		}
	}
	return true;
}

void ServerPickingSystem::setScoreEffect( Entity* p_player, Transform* p_moduleTransform, int p_score )
{
	OnHitScoreEffectPacket fxPacket;
	fxPacket.score = p_score;
	fxPacket.position = p_moduleTransform->getTranslation();
	fxPacket.angle = p_moduleTransform->getRotation();	
	m_effectbuffer->enqueueEffect(p_player,fxPacket);
}
//Rotation
void ServerPickingSystem::addRotationEvent(int direction, int client)
{
	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (client == m_pickComponents[i].m_clientIndex)
		{
			m_pickComponents[i].m_rotationDirection = direction;
			break;
		}
	}
}
void ServerPickingSystem::add90RotationEvent(int direction, int client)
{
	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (client == m_pickComponents[i].m_clientIndex)
		{
			if (m_pickComponents[i].getLatestPick() >= 0)
			{
				Entity* e = m_world->getEntity(m_pickComponents[i].getLatestPick());
				if (e)
				{
					ShipModule*  module = static_cast<ShipModule*>(e->getComponent(ComponentType::ShipModule));
					if (module)
					{
						module->m_rotation += direction * 3.14159f / 2.0f;
					}
				}

			}
			else if (m_pickComponents[i].m_latestAttached >= 0)
			{
				Entity* e = m_world->getEntity(m_pickComponents[i].m_latestAttached);
				if (e)
				{
					ShipModule*  module = static_cast<ShipModule*>(e->getComponent(ComponentType::ShipModule));
					if (module)
					{
						module->m_rotation += direction * 3.14159f / 2.0f;
						rotateModule(e, 0);
					}
				}

			}
			break;
		}
	}
}


void ServerPickingSystem::rotateModule(Entity* p_module, int p_dir)
{
	ShipModule*  module = static_cast<ShipModule*>(p_module->getComponent(ComponentType::ShipModule));
	module->m_rotation += m_world->getDelta()*p_dir;

	PhysicsBody* moduleBody = static_cast<PhysicsBody*>(p_module->getComponent(ComponentType::PhysicsBody));

	Entity* parent = m_world->getEntity(module->m_parentEntity);
	PhysicsBody* parentBody = static_cast<PhysicsBody*>(parent->getComponent(ComponentType::PhysicsBody));

	ConnectionPointSet* parentCon =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(parent,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	int parentSlot = 0;
	for (unsigned int i = 1; i < parentCon->m_connectionPoints.size(); i++)
	{
		if (parentCon->m_connectionPoints[i].cpConnectedEntity == p_module->getIndex())
		{
			parentSlot = i;
			break;
		}
	}

	ConnectionPointSet* childCon =
		static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent(p_module,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	int childSlot = 0;
	for (unsigned int i = 1; i < childCon->m_connectionPoints.size(); i++)
	{
		if (childCon->m_connectionPoints[i].cpConnectedEntity == parent->getIndex())
		{
			childSlot = i;
			break;
		}
	}

	AglMatrix transform = offsetTemp(parent, parentCon->m_connectionPoints[parentSlot].cpTransform*parentBody->getOffset().inverse(), 
		childCon->m_connectionPoints[childSlot].cpTransform*moduleBody->getOffset().inverse(), module->m_rotation);

	PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
	Body* body = ps->getController()->getBody(moduleBody->m_id);
	body->setTransform(transform);
}

//Send information about the Selection marker
void ServerPickingSystem::updateSelectionMarker(PickComponent& p_ray)
{
	if (p_ray.getLatestPick() >= 0 && p_ray.m_targetEntity >= 0 && p_ray.m_targetEntity != p_ray.getLatestPick())
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		//Module
		Entity* module = m_world->getEntity(p_ray.getLatestPick());
		Transform* moduleTransform = static_cast<Transform*>(module->getComponent(
			ComponentType::Transform));
		ShipModule* shipModule = static_cast<ShipModule*>(module->getComponent(
			ComponentType::ShipModule));
		PhysicsBody* moduleBody = static_cast<PhysicsBody*>(module->getComponent(
			ComponentType::PhysicsBody));
		//NetworkSynced* networkComp = static_cast<NetworkSynced*>
		//	(module->getComponent(ComponentType::NetworkSynced));
		//PlayerComponent* scoreComponent = m_playerSystem->getPlayerCompFromNetworkComp(networkComp);

		//Find module connection point
		ConnectionPointSet* conPoints = static_cast<ConnectionPointSet*>(module->getComponent(ComponentType::ConnectionPointSet));

		//Don't allow connection if the module has no connection points
		if (!conPoints)
			return;

		int sel = -1;
		for (unsigned int i = 0; i < conPoints->m_connectionPoints.size(); i++)
		{
			if (conPoints->m_connectionPoints[i].cpConnectedEntity < 0)
			{
				sel = i;
				break;
			}
		}

		//Don't allow connection if the module doesn't have any free connection points
		if (sel < 0)
			return;

		//Target
		Entity* target = m_world->getEntity(p_ray.m_targetEntity);
		Entity* ship = target;

		bool invalidTarget = false;

		while (ship->getComponent(ComponentType::ShipModule))
		{
			ShipModule* intermediate = static_cast<ShipModule*>(ship->getComponent(
				ComponentType::ShipModule));

			if (intermediate->m_parentEntity < 0)
			{
				invalidTarget = true;
				break;
			}

			ship = m_world->getEntity(intermediate->m_parentEntity);
		}

		if (invalidTarget)
		{
			SelectionMarkerUpdatePacket smup;
			smup.targetNetworkIdentity = -1;
			smup.transform = AglMatrix::identityMatrix();
			m_server->unicastPacket(smup.pack(), p_ray.m_clientIndex);
			return;
		}

		PhysicsBody* shipBody = static_cast<PhysicsBody*>(ship->getComponent(
			ComponentType::PhysicsBody));

		PhysicsBody* targetBody = static_cast<PhysicsBody*>(target->getComponent(
			ComponentType::PhysicsBody));

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(target,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		CompoundBody* comp = (CompoundBody*)physX->getController()->getBody(shipBody->m_id);
		RigidBody* r = (RigidBody*)physX->getController()->getBody(moduleBody->m_id);

		AglMatrix transform = offsetTemp(target, cps->m_connectionPoints[p_ray.m_targetSlot].cpTransform*targetBody->getOffset().inverse(), 
			conPoints->m_connectionPoints[sel].cpTransform*moduleBody->getOffset().inverse(), shipModule->m_rotation);

		MeshOffsetTransform* moff = static_cast<MeshOffsetTransform*>(module->getComponent(ComponentType::MeshOffsetTransform));

		transform = transform * comp->GetWorld();
		transform = moduleBody->getOffset().inverse()*transform;

		///Send the update packet 
		NetworkSynced* shipNetworkSynced = static_cast<NetworkSynced*>(
			ship->getComponent(ComponentType::NetworkSynced));

		Transform* trans = static_cast<Transform*>(
			ship->getComponent(ComponentType::Transform));

		SelectionMarkerUpdatePacket smup;
		smup.targetNetworkIdentity = module->getIndex();
		smup.transform = transform;//trans->getMatrix();
		m_server->unicastPacket(smup.pack(), shipNetworkSynced->getNetworkOwner());
	}
	else
	{
		SelectionMarkerUpdatePacket smup;
		smup.targetNetworkIdentity = -1;
		smup.transform = AglMatrix::identityMatrix();
		m_server->unicastPacket(smup.pack(), p_ray.m_clientIndex);
	}
}

void ServerPickingSystem::unsetPick(PickComponent& p_ray)
{
	HighlightEntityPacket high;
	high.target = p_ray.getLatestPick();
	high.on = false;
	m_server->unicastPacket(high.pack(), p_ray.m_clientIndex);
	p_ray.setLatestPick(-1);
}

void ServerPickingSystem::setModuleUsedStatusEffect( Entity* p_module )
{
	NetworkSynced* networkSynced = static_cast<NetworkSynced*>(
		p_module->getComponent(ComponentType::NetworkSynced));

	ModuleStatusEffectPacket fxPacket(ModuleStatusEffectPacket::UNUSEDMODULE_STATUS,
		ModuleStatusEffectPacket::OFF,
		networkSynced->getNetworkOwner());
	m_effectbuffer->enqueueEffect(fxPacket);
}
