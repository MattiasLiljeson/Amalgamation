#pragma once

#include <EntitySystem.h>
#include "MainCamera.h"
#include "Transform.h"
#include "PickComponent.h"
#include "TcpClient.h"

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
class PickingSystem: public EntitySystem
{
public:
	PickingSystem(TcpClient* p_client);
	~PickingSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	int createRay();
	void setRay(int p_index, AglVector3 p_o, AglVector3 p_dir);
	void checkPicking(PickComponent* p_pickComponent);
private:
	TcpClient* m_client;
};