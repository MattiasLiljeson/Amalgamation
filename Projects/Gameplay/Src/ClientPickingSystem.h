#pragma once

#include <EntitySystem.h>
#include "MainCamera.h"
#include "Transform.h"
#include "PickComponent.h"
#include "TcpClient.h"

// =======================================================================================
//                                      ClientPickingSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Creates rays and sends to server
///        
/// # ClientPickingSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ClientPickingSystem: public EntitySystem
{
public:
	ClientPickingSystem(TcpClient* p_client);
	~ClientPickingSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	TcpClient* m_client;
};