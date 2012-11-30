#include "ProcessMessageClientConnected.h"

ProcessMessageClientConnected::ProcessMessageClientConnected()
	: ProcessMessage( MessageType::CLIENT_CONNECTED, NULL )
{
	socket = NULL;
}

ProcessMessageClientConnected::ProcessMessageClientConnected(
	ThreadSafeMessaging* p_sender, tcp::socket* p_socket )
	: ProcessMessage( MessageType::CLIENT_CONNECTED, p_sender )
{
	socket = p_socket;
}

ProcessMessageClientConnected::~ProcessMessageClientConnected()
{
}
