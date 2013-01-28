#pragma once

#include <EntitySystem.h>

class TcpServer;

// =======================================================================================
//                                      ServerUpdateSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkUpdateSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ServerUpdateSystem : public EntitySystem
{
public:
	ServerUpdateSystem( TcpServer* p_server );

	~ServerUpdateSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

	virtual void removed( Entity* p_entity );


private:
	TcpServer* m_server;

};