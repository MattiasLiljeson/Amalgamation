// =======================================================================================
//	NetworkConnectToServerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkConnectToServerSystem
/// Detailed description...
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <EntitySystem.h>

class NetworkConnectToServerSystem: public EntitySystem
{
public:
	NetworkConnectToServerSystem();

	~NetworkConnectToServerSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

};