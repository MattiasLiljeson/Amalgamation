#include "ServerPickingSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "RayPacket.h"

ServerPickingSystem::ServerPickingSystem()
	: EntitySystem(SystemType::ServerPickingSystem)
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
	//DOES NOT PROCESS ENTITIES!

	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < m_pickComponents.size(); i++)
	{
		handleRay(m_pickComponents[i]);
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
void ServerPickingSystem::handleRay(PickComponent& p_pc)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	vector<unsigned int> cols = physX->getPhysicsController()->LineCollidesWith(p_pc.m_rayIndex);
	if (cols.size() > 0)
	{
		p_pc.m_latestPick = cols[0];
	}
	else
		p_pc.m_latestPick = -1;
}