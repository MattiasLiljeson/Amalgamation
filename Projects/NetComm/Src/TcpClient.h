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
	Packet popNewPacket();

	void sendPacket( Packet p_packet );

	void processMessages();

	int getId();
	int* getIdPointer();
	void setId( int p_id );

	unsigned int getNumberOfSentPackets();
	unsigned int getNumberOfReceivedPackets();
	unsigned int getTotalDataSent();
	unsigned int getTotalDataReceived();
	unsigned int getTotalNumberOfOverflowPackets();

	void resetNumberOfSentPackets();
	void resetNumberOfReceivedPackets();
	void resetTotalDataSent();
	void resetTotalDataReceived();

	float getServerTimeAhead() const;
	void setServerTimeAhead( float p_timeAhead );

	float getPingToServer() const;
	void setPingToServer( float p_ping );

private:
	boost::asio::io_service* m_ioService;

	TcpConnecterProcess* m_connecterProcess;

	int m_numConnections;
	TcpCommunicationProcess* m_communicationProcess;

	queue< Packet > m_newPackets;

	int m_id;

	unsigned int m_numberOfSentPackets;
	unsigned int m_numberOfReceivedPackets;
	unsigned int m_totalDataSent;
	unsigned int m_totalDataReceived;
	unsigned int m_totalNumberOfOverflowPackets;

	float m_serverTimeAhead;
	float m_pingToServer;

};