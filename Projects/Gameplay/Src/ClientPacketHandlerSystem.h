#pragma once

#include <EntitySystem.h>
#include <Packet.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include "LevelPieceFileData.h"
#include <queue>
#include "EntityUpdatePacket.h"
#include "ParticleSystemCreationInfo.h"
#include "ParticleUpdatePacket.h"

using namespace std;

class TcpClient;
class EntityCreationPacket;
class EntityDeletionPacket;
class GameStateSystem;
class ClientStateSystem;

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
	void handleEntityDeletionPacket(EntityDeletionPacket p_packet);

	void updateCounters();

	void updateInitialPacketLossDebugData();
	void updateBroadcastPacketLossDebugData( unsigned int p_packetIdentifier );

	//Added by Anton
	void handleBatch();

	void handleParticleSystemCreation( const ParticleSystemCreationInfo& p_creationInfo );
	void handleParticleSystemUpdate( const ParticleUpdatePacket& p_data );
	void printPacketTypeNotHandled(string p_stateName, int p_packetType);

	void handleIngameState();
	void handleLobby();
	void handleLoading();
	void handleFinishedLoading();
	void handleResults();
private:
	ClientStateSystem* m_gameState;
	TcpClient* m_tcpClient;

	float m_currentPing;

	unsigned int m_totalNetworkSynced;
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
	unsigned int m_lastBroadcastPacketIdentifier;

	unsigned int m_totalBroadcastPacketLost;

	queue<int> m_staticPropIdentities;

	//Added by Anton
	vector<EntityUpdatePacket> m_batch;
};