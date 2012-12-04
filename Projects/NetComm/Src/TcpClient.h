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

#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio::ip;

class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	bool connectToServer( string p_address, string p_port );

	bool hasActiveConnection();

private:
	boost::asio::io_service* m_ioService;
	tcp::socket* m_activeSocket;

	int m_numConnections;

};