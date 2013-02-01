#pragma once

#include <EntitySystem.h>
#include "StandardRocket.h"
#include "Transform.h"
#include "TcpServer.h"

// =======================================================================================
//                                      RocketControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief
///        
/// # RocketControllerSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class RocketControllerSystem: public EntitySystem
{
public:
	RocketControllerSystem(TcpServer* p_server);
	~RocketControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	float m_turnPower;
	float m_enginePower;
	TcpServer* m_server;
};