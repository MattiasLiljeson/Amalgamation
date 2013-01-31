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

ServerPickingSystem::ServerPickingSystem(TcpServer* p_server)
	: EntitySystem(SystemType::ServerPickingSystem, 1, ComponentType::ShipModule)
{
	m_server = p_server;
}


ServerPickingSystem::~ServerPickingSystem()
{
}

void ServerPickingSystem::initialize()
{
}

void ServerPickingSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		if (m_pickComponents[i].m_latestPick >= 0)
		{
			project(m_world->getEntity(m_pickComponents[i].m_latestPick), m_pickComponents[i]);
		}
		else
		{
			if (m_pickComponents[i].m_active)
				handleRay(m_pickComponents[i], p_entities);
			else if (m_pickComponents[i].m_selection >= 0)
			{
				Entity* SelectionSphere = m_world->getEntity(m_pickComponents[i].m_selection);
				Transform* SelectionSphereTransform = static_cast<Transform*>(SelectionSphere->getComponent(ComponentType::Transform));
				SelectionSphereTransform->setScale(AglVector3(0, 0, 0));
			}
		}
		if (m_pickComponents[i].m_selection < 0)
		{
			//Create the selection highlighter sphere
			Entity* entity = m_world->createEntity();

			Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::identity(), AglVector3(1, 1, 1));
			entity->addComponent( ComponentType::Transform, t);
			m_world->addEntity(entity);
			m_pickComponents[i].m_selection = entity->getIndex();

			EntityCreationPacket data;
			data.entityType		= static_cast<char>(EntityType::SelectionSphere);
			data.owner			= -1;
			data.networkIdentity = entity->getIndex();
			data.translation	= t->getTranslation();
			data.rotation		= t->getRotation();
			data.scale			= t->getScale();
			data.meshInfo		= 1;

			entity->addComponent(ComponentType::NetworkSynced, 
				new NetworkSynced( entity->getIndex(), -1, EntityType::SelectionSphere));

			m_server->broadcastPacket(data.pack());
		}
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
				m_pickComponents[i].m_latestPick = -1;
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
			//Release the picked module
			m_pickComponents[i].m_latestPick = -1;
			m_pickComponents[i].m_active = false;
			return;
		}
	}
}
void ServerPickingSystem::handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities)
{
	if (p_pc.m_active)
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		int col = physX->getPhysicsController()->LineClosestCollision(p_pc.m_rayIndex);
		if (col >= 0)
		{
			for (unsigned int i = 0; i < p_entities.size(); i++)
			{
				PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
				if (pb && pb->m_id == col)
				{
					//Found a pick
					p_pc.m_latestPick = p_entities[i]->getIndex();

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
						p_pc.m_latestPick = -1;
					}
					break;
				}
			}
		}
	}
}
void ServerPickingSystem::project(Entity* toProject, PickComponent& p_ray)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	AglVector3 origin;
	AglVector3 dir;
	physX->getController()->GetRay(p_ray.m_rayIndex, origin, dir);


	PhysicsBody* projectBody = static_cast<PhysicsBody*>(toProject->getComponent(ComponentType::PhysicsBody));
	Body* body = physX->getController()->getBody(projectBody->m_id);

	AglVector3 vec = body->GetWorld().GetTranslation() - origin;
	float len = vec.length();
	AglVector3 dest = origin + dir * p_ray.m_preferredDistance;


	ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

	//Handle the ship
	Entity* ship = sms->findShip(p_ray.m_clientIndex);

	PhysicsBody* shipBody = static_cast<PhysicsBody*>(ship->getComponent(ComponentType::PhysicsBody));
	Body* physicalShipBody = physX->getController()->getBody(shipBody->m_id);

	AglVector3 sphereCenter = physicalShipBody->GetWorld().GetTranslation();

	float radius = 4; //Hard coded radius for now

	dir = dest - sphereCenter;
	dir.normalize();
	dest = sphereCenter + dir * radius;

	AglVector3 vel = body->GetVelocity();

	float t = getT(origin, dir, sphereCenter, radius);
	if (t > 0)
		dest = origin + dir*t;
	body->AddImpulse(-vel + (dest - body->GetWorld().GetTranslation())*10);

	//Fix selection sphere
	Entity* SelectionSphere = m_world->getEntity(p_ray.m_selection);
	Transform* SelectionSphereTransform = static_cast<Transform*>(SelectionSphere->getComponent(ComponentType::Transform));
	SelectionSphereTransform->setTranslation(closestConnectionPoint(dest, ship, p_ray));
	if (p_ray.m_targetEntity >= 0)
		SelectionSphereTransform->setScale(AglVector3(1, 1, 1));
	else
		SelectionSphereTransform->setScale(AglVector3(0, 0, 0));
}
AglVector3 ServerPickingSystem::closestConnectionPoint(AglVector3 p_position, Entity* p_entity, PickComponent& p_pc)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	ConnectionPointSet* conPoints = static_cast<ConnectionPointSet*>(p_entity->getComponent(ComponentType::ConnectionPointSet));
	Transform* transform = static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform));

	vector<pair<int, Entity*>> free = getFreeConnectionPoints(conPoints, p_entity);

	PhysicsBody* phyBody = static_cast<PhysicsBody*>(p_entity->getComponent(ComponentType::PhysicsBody));
	Body* b = physX->getController()->getBody(phyBody->m_id);
	AglVector3 parentPos = b->GetWorld().GetTranslation();

	AglVector3 closest = AglVector3(0, 0, 0);
	if (free.size() > 0)
	{
		AglVector3 pos = (conPoints->m_connectionPoints[free[0].first].cpTransform*transform->getMatrix()).GetTranslation();
		closest = pos;
		p_pc.m_targetEntity = free[0].second->getIndex();
		p_pc.m_targetSlot = free[0].first;
		for (unsigned int i = 0; i < free.size(); i++)
		{
			conPoints = static_cast<ConnectionPointSet*>(free[i].second->getComponent(ComponentType::ConnectionPointSet));
			transform = static_cast<Transform*>(free[i].second->getComponent(ComponentType::Transform));
			AglVector3 pos = (conPoints->m_connectionPoints[free[i].first].cpTransform*transform->getMatrix()).GetTranslation();
			if (AglVector3::lengthSquared(pos-p_position) < AglVector3::lengthSquared(closest-p_position))
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


vector<pair<int, Entity*>> ServerPickingSystem::getFreeConnectionPoints(ConnectionPointSet* p_set, Entity* p_parent)
{
	vector<pair<int, Entity*>> free;
	for (unsigned int i = 0; i < p_set->m_connectionPoints.size(); i++)
	{
		if (p_set->m_connectionPoints[i].cpConnectedEntity < 0)
			free.push_back(pair<int, Entity*>(
			i, p_parent));
		else
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
	return free;
}

AglMatrix ServerPickingSystem::offsetTemp(Entity* p_entity, AglMatrix p_base)
{
	AglMatrix transform = p_base;
	ShipModule* module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));
	while (module)
	{
		Entity* parent = m_world->getEntity(module->m_parentEntity);

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(parent,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		unsigned int ind = 0;
		for (unsigned int i = 1; i < cps->m_connectionPoints.size(); i++)
		{
			if (cps->m_connectionPoints[i].cpConnectedEntity == p_entity->getIndex())
				ind = i;
		}

		transform = transform * cps->m_connectionPoints[ind].cpTransform;
		module = static_cast<ShipModule*>(parent->getComponent(ComponentType::ShipModule));
		p_entity = parent;
	}
	return transform;
}


void ServerPickingSystem::attemptConnect(PickComponent& p_ray)
{
	if (p_ray.m_latestPick >= 0 && p_ray.m_targetEntity >= 0)
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		//Module
		Entity* module = m_world->getEntity(p_ray.m_latestPick);
		ShipModule* shipModule = static_cast<ShipModule*>(module->getComponent(ComponentType::ShipModule));
		PhysicsBody* moduleBody = static_cast<PhysicsBody*>(module->getComponent(ComponentType::PhysicsBody));

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
		while (ship->getComponent(ComponentType::ShipModule))
		{
			ShipModule* intermediate = static_cast<ShipModule*>(ship->getComponent(ComponentType::ShipModule));
			ship = m_world->getEntity(intermediate->m_parentEntity);
		}
		PhysicsBody* shipBody = static_cast<PhysicsBody*>(ship->getComponent(ComponentType::PhysicsBody));

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(target,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));


		CompoundBody* comp = (CompoundBody*)physX->getController()->getBody(shipBody->m_id);
		RigidBody* r = (RigidBody*)physX->getController()->getBody(moduleBody->m_id);

		//WARNING: DOES NOT HANDLE TRANSFORMATION RELATED TO CHILD CONNECTION POINT RIGHT NOW


		//Parent transform
		AglMatrix transform = offsetTemp(target, cps->m_connectionPoints[p_ray.m_targetSlot].cpTransform);

		//Child Transform
		AglMatrix childTransform = conPoints->m_connectionPoints[sel].cpTransform;
		AglQuaternion rot = AglQuaternion::rotateToFrom(childTransform.GetForward(), -transform.GetForward());
		AglMatrix finalTransform = AglMatrix::createRotationMatrix(rot);

		AglVector3 childTrans = childTransform.GetTranslation();
		rot.transformVector(childTrans);
		finalTransform.SetTranslation(transform.GetTranslation() + childTrans);

		physX->getController()->AttachBodyToCompound(comp, r, finalTransform);
		
		//Set the parent connection point
		cps->m_connectionPoints[p_ray.m_targetSlot].cpConnectedEntity = module->getIndex();
		
		//Set the module connection point
		conPoints->m_connectionPoints[sel].cpConnectedEntity = ship->getIndex();

		shipModule->m_parentEntity = target->getIndex();

		moduleBody->setParentId(shipBody->m_id);
	}
}
bool ServerPickingSystem::attemptDetach(PickComponent& p_ray)
{
	//Add Check so that modules with other modules connected to them
	//cannot be removed

	if (p_ray.m_latestPick >= 0)
	{
		PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
			SystemType::PhysicsSystem));

		//Module
		Entity* module = m_world->getEntity(p_ray.m_latestPick);
		ShipModule* shipModule = static_cast<ShipModule*>(module->getComponent(ComponentType::ShipModule));
		PhysicsBody* moduleBody = static_cast<PhysicsBody*>(module->getComponent(ComponentType::PhysicsBody));

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
		}
	}
	return true;
}