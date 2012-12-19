#include "ProcessMessageSocketDisconnected.h"

ProcessMessageSocketDisconnected::ProcessMessageSocketDisconnected()
	: ProcessMessage( MessageType::SOCKET_DISCONNECTED, 0 )
{
}

ProcessMessageSocketDisconnected::ProcessMessageSocketDisconnected( 
	ThreadSafeMessaging* p_sender,
	int p_processId )
	: ProcessMessage( MessageType::SOCKET_DISCONNECTED, p_sender )
{
	processId = p_processId;
}

ProcessMessageSocketDisconnected::~ProcessMessageSocketDisconnected()
{
}