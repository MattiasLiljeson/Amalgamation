#pragma once

#include <EntitySystem.h>
#include "Transform.h"
#include "PickComponent.h"

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
	ServerPickingSystem();
	~ServerPickingSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void setRay(int p_index, AglVector3 p_o, AglVector3 p_d);
private:
	vector<PickComponent> m_pickComponents;
private:
	void handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities);
};