// =======================================================================================
//                                      TcpClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpClient
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>

#include "Packet.h"
#include "TcpConnecterProcess.h"
#include "ThreadSafeMessaging.h"

namespace boost
{
	namespace asio
	{
		class io_service;
	};
};

using namespace std;

class TcpCommunicationProcess;

class TcpClient: public ThreadSafeMessaging
{
public:
	TcpClient();
	~TcpClient();

	bool connectToServer( string p_address, string p_port );

	void connectToServerAsync(  string p_address, string p_port );

	void disconnect();

	bool hasActiveConnection();

	bool hasNewPackets();
	unsigned int newPacketsCount();
	Packet popNewPacket(); // Deprecated and obsolete.
	void popFrontPacket();

	Packet& getFrontPacket();

	void sendPacket( Packet& p_packet );

	void processMessages();

	int getId();
	int* getIdPointer();
	void setId( int p_id );

	int getPlayerID();
	void setPlayerID( int p_playerId );

	string getPlayerName();
	void setPlayerName( const string& p_playerName );

	unsigned int getNumberOfSentPackets();
	unsigned int getNumberOfReceivedPackets();
	unsigned int getTotalDataSent();
	unsigned int getTotalDataReceived();
	unsigned int getTotalNumberOfOverflowPackets();
	unsigned int getTotalPacketsReceivedInCommProcess();
	unsigned int getTotalPacketsSentInCommProcess();
	void askForCommProcessInfo();

	void resetNumberOfSentPackets();
	void resetNumberOfReceivedPackets();
	void resetTotalDataSent();
	void resetTotalDataReceived();

	float getServerTimeAhead() const;
	void setServerTimeAhead( float p_timeAhead );

	float getPingToServer() const;
	void setPingToServer( float p_ping );
	void setServerName(const string& p_serverName );
	string getServerName();
	void setServerGameTime(int p_gameTime);
	int getServerGameTime();

private:
	boost::asio::io_service* m_ioService;

	TcpConnecterProcess* m_connecterProcess;

	int m_numConnections;
	TcpCommunicationProcess* m_communicationProcess;

	queue< Packet > m_newPackets;

	int m_id;
	int m_playerId;
	string m_playerName;
	unsigned int m_numberOfSentPackets;
	unsigned int m_totalPacketsPopped;
	unsigned int m_totalDataSent;
	unsigned int m_totalDataReceived;
	unsigned int m_totalNumberOfOverflowPackets;
	unsigned int m_totalPacketsReceivedInCommProcess;
	unsigned int m_totalPacketsSentInCommProcess;

	float m_serverTimeAhead;
	float m_pingToServer;
	string m_serverName;
	int m_serverGameTime;
};