#pragma once

#include <EntitySystem.h>
#include "AglVector3.h"
#include "ConnectionPointSet.h"
#include "TcpServer.h"

extern AglVector3 minPhys;//(FLT_MAX, FLT_MAX, FLT_MAX);
extern AglVector3 maxPhys;//(FLT_MIN, FLT_MIN, FLT_MIN);

class PhysicsController;
class PhysicsBody;
class Body;

// =======================================================================================
//                                      PhysicsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for handling communication with the physics library
///        
/// # PhysicsSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class PhysicsSystem: public EntitySystem
{
public:
	PhysicsSystem(TcpServer* p_server);
	~PhysicsSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse);
	PhysicsController* getPhysicsController();

	PhysicsController* getController(){ return m_physicsController; }

	Entity* getEntity(int p_bodyID)
	{
		if (m_entityMap.size() <= p_bodyID || m_entityMap[p_bodyID] < 0)
			return NULL;
		return m_world->getEntity(m_entityMap[p_bodyID]);
	}

private:
	void initializeEntity(Entity* p_entity);
	void handleCompoundBodyDependencies(Entity* p_entity);

	//Temp
	void addModulesToShip(PhysicsBody* p_body, AglVector3 p_position);
	void queryShipCollision(Entity* ship, const vector<Entity*>& p_others);

	vector<pair<ConnectionPoint*, Entity*>> getFreeConnectionPoints(ConnectionPointSet* p_set, Entity* p_parent);

	AglMatrix offset(Entity* p_entity, AglMatrix p_base);
private:
	PhysicsController* m_physicsController;
	vector<int>	m_entityMap;
	TcpServer* m_server;

};