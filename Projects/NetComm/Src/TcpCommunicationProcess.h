#pragma once

#include <boost/asio.hpp>

#include "ProcessMessageSendPacket.h"
#include "ProcessThread.h"

using namespace boost::asio::ip;

class TcpCommunicationProcess: public ProcessThread
{
public:
	TcpCommunicationProcess( tcp::socket* p_socket );

	~TcpCommunicationProcess();

	void body();

private:
	tcp::socket* m_activeSocket;

	bool m_running;

};