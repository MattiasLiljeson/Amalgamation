#pragma once

#include <EntitySystem.h>
#include <Packet.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <ctime>
#include <Windows.h>

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

struct NetworkEntityUpdatePacket
{
	char			networkType;
	int				networkId;
	AglVector3		position;
	AglQuaternion	rotation;
	AglVector3		scale;
};

struct NetworkScoreUpdatePacket
{
	int networkId;
	int score;
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
	NetworkEntityUpdatePacket	readUpdatePacket(Packet& p_packet);
	NetworkScoreUpdatePacket	readScorePacket(Packet& p_packet);

private:
	TcpClient* m_tcpClient;

	float m_timer;
	float m_timerStartValue;
	SYSTEMTIME m_timestamp;
	int m_currentPing;

};