#pragma once

#include <EntitySystem.h>
#include <Packet.h>
#include <AglVector3.h>
#include <AglQuaternion.h>

class TcpClient;

struct NetworkEntityCreationPacket
{
	char			networkType;
	int				owner;
	int				networkId;
	AglVector3		position;
	AglQuaternion	rotation;
	AglVector3		scale;
};


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
	NetworkEntityCreationPacket readCreationPacket(Packet& p_packet);

private:
	TcpClient* m_tcpClient;

};