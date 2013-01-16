#include "ServerPickingSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "RayPacket.h"
#include "PhysicsBody.h"
#include "ShipManagerSystem.h"

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

ServerPickingSystem::ServerPickingSystem()
	: EntitySystem(SystemType::ServerPickingSystem, 1, ComponentType::ShipModule)
{
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
		m_pickComponents[i].m_active = max(0, m_pickComponents[i].m_active-dt);
		if (m_pickComponents[i].m_latestPick >= 0)
		{
			project(m_world->getEntity(m_pickComponents[i].m_latestPick), m_pickComponents[i]);
		}
		else
		{
			if (m_pickComponents[i].m_active)
				handleRay(m_pickComponents[i], p_entities);
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
				m_pickComponents[i].m_latestPick = -1;
			return;
		}
	}
}
void ServerPickingSystem::handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	vector<unsigned int> cols = physX->getPhysicsController()->LineCollidesWith(p_pc.m_rayIndex);
	if (cols.size() > 0)
	{
		for (unsigned int i = 0; i < p_entities.size(); i++)
		{
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			if (pb && pb->m_id == cols[0])
			{
				p_pc.m_latestPick = p_entities[i]->getIndex();

				AglVector3 origin;
				AglVector3 dir;
				physX->getController()->GetRay(p_pc.m_rayIndex, origin, dir);

				AglVector3 d = physX->getController()->getBody(cols[0])->GetWorld().GetTranslation()-origin;
				p_pc.m_preferredDistance = d.length();


				break;
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
	{
		dest = origin + dir*t;
		body->AddImpulse(-vel + (dest - body->GetWorld().GetTranslation())*10);
	}
}