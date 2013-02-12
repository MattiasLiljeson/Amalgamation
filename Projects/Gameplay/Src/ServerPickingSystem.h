#pragma once

#include <EntitySystem.h>
#include "Transform.h"
#include "PickComponent.h"
#include "TcpServer.h"
#include "ConnectionPointSet.h"

// =======================================================================================
//                                      ServerPickingSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Creates rays and sends to server
///        
/// # ServerPickingSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ServerPickingSystem: public EntitySystem
{
public:
	ServerPickingSystem(TcpServer* p_server);
	~ServerPickingSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void setRay(int p_index, AglVector3 p_o, AglVector3 p_d);
	void setEnabled(int p_index, bool p_value);
	void setReleased(int p_index);
private:
	vector<PickComponent> m_pickComponents;
	TcpServer* m_server;

	//Temp
	float mrota;
private:
	void handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities);
	void project(Entity* toProject, PickComponent& p_ray);
	AglVector3 closestConnectionPoint(AglVector3 p_position, Entity* p_entity, PickComponent& p_pc);
	void attemptConnect(PickComponent& p_ray);
	bool attemptDetach(PickComponent& p_ray);
	AglMatrix offsetTemp(Entity* p_entity, AglMatrix p_base, AglMatrix p_offset);
	vector<pair<int, Entity*>> getFreeConnectionPoints(ConnectionPointSet* p_set, Entity* p_parent);
};