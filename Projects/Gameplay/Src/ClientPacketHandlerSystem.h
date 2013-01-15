#pragma once

#include <EntitySystem.h>
#include <Packet.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <queue>

using namespace std;

class TcpClient;
class EntityCreationPacket;

struct NetworkScoreUpdatePacket
{
	int networkId;
	int score;
};

// =======================================================================================
//                                      ClientPacketHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkCommunicatorSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ClientPacketHandlerSystem: public EntitySystem
{
public:
	ClientPacketHandlerSystem( TcpClient* p_tcpClient );

	~ClientPacketHandlerSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

private:
	//NetworkEntityCreationPacket readCreationPacket(Packet& p_packet);
	NetworkScoreUpdatePacket	readScorePacket(Packet& p_packet);

	void handleWelcomePacket(Packet p_packet);
	void handleEntityCreationPacket(EntityCreationPacket p_packet);

	void updateCounters();

	void updatePacketLossDebugData();

private:
	TcpClient* m_tcpClient;
	float m_currentPing;

	unsigned int m_numberOfSentPackets;
	unsigned int m_numberOfReceivedPackets;
	unsigned int m_totalDataSent;
	unsigned int m_totalDataReceived;
	unsigned int m_dataSentPerSecond;
	unsigned int m_dataReceivedPerSecond;
	unsigned int m_dataSentCounter;
	unsigned int m_dataReceivedCounter;
	unsigned int m_totalNumberOfOverflowPackets;
	unsigned int m_totalNumberOfStaticPropPacketsReceived;
	vector< pair<int,int> > m_staticPropIdentitiesForAntTweakBar;

	queue<int> m_staticPropIdentities;
};