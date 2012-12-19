#pragma once

#include <boost/asio/ip/tcp.hpp>
//#include "stdafx.h"
//	#include "DebugUtil.h" // Doesn't work :(

#include "ProcessThread.h"

namespace boost
{
	namespace asio
	{
		class io_service;
		
		//namespace ip
		//{
		//	class tcp;
		//};

		//template<class T1, class T2>
		//class basic_stream_socket<T1, T2>;
		//template<class T>
		//class stream_socket_service<T>;

		//template<>
		//class basic_stream_socket<ip::tcp, stream_socket_service<ip::tcp>>;
	};
};

using namespace boost::asio::ip;

class ThreadSafeMessaging;

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
	vector<char> m_reserveBuffer;
	unsigned int m_packetRestSize;

	ThreadSafeMessaging* m_parent;

};