#include "ProcessMessage.h"
#include "ThreadSafeMessaging.h"

ProcessMessage::ProcessMessage( MessageType::MESSAGE_TYPE p_type,
							   ThreadSafeMessaging* p_sender )
{
	type = p_type;
	sender = p_sender;
}

ProcessMessage::~ProcessMessage()
{
}
