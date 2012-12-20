#pragma once

#include <EntitySystem.h>

class TcpServer;

// =======================================================================================
//                                      NetworkUpdateSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkUpdateSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class NetworkUpdateSystem: public EntitySystem
{
public:
	NetworkUpdateSystem( TcpServer* p_server );

	~NetworkUpdateSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

private:
	TcpServer* m_server;

};