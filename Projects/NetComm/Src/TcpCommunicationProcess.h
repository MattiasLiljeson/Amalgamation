#pragma once

#include <boost/asio.hpp>

#include "ProcessMessageSendPacket.h"
#include "ProcessMessageSocketDisconnected.h"
#include "ProcessMessageReceivePacket.h"
#include "ProcessThread.h"
#include "ThreadSafeMessaging.h"
#include "StringSplit.h"

using namespace boost::asio::ip;

class TcpCommunicationProcess: public ProcessThread
{
public:
	TcpCommunicationProcess( ThreadSafeMessaging* p_parent, tcp::socket* p_socket,
		boost::asio::io_service* p_ioService );

	~TcpCommunicationProcess();

	void body();

	void processMessages();

private:
	void startPacketReceiveCallback();

	void onReceivePacket( const boost::system::error_code& p_error,
		size_t p_bytesTransferred );

private:
	tcp::socket* m_activeSocket;

	bool m_running;

	boost::asio::io_service* m_ioService;

	char* m_asyncData;
	unsigned int m_asyncDataLength;
	unsigned int m_asyncDataCapacity;

	ThreadSafeMessaging* m_parent;

};