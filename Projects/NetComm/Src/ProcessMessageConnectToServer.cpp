#include "ProcessMessageConnectToServer.h"

ProcessMessageConnectToServer::ProcessMessageConnectToServer()
	: ProcessMessage( MessageType::CONNECT_TO_SERVER, NULL )
{
	socket = NULL;
}

ProcessMessageConnectToServer::ProcessMessageConnectToServer(
	ThreadSafeMessaging* p_sender,
	boost::asio::ip::tcp::socket* p_socket )
	: ProcessMessage( MessageType::CONNECT_TO_SERVER, p_sender )
{
	socket = p_socket;
}

ProcessMessageConnectToServer::~ProcessMessageConnectToServer()
{
}
