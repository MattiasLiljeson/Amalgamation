#pragma once

#include <EntitySystem.h>
#include "StandardMine.h"
#include "Transform.h"
#include "TcpServer.h"

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
class MineControllerSystem: public EntitySystem
{
public:
	MineControllerSystem(TcpServer* p_server);
	~MineControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	TcpServer* m_server;
};