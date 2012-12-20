#pragma once

#include <EntitySystem.h>
class TcpClient;

// =======================================================================================
//                                      NetworkCommunicatorSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkCommunicatorSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class NetworkCommunicatorSystem: public EntitySystem
{
public:
	NetworkCommunicatorSystem( TcpClient* p_tcpClient );

	~NetworkCommunicatorSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

private:
	TcpClient* m_tcpClient;

};