#include "ProcessMessage.h"


ProcessMessage::ProcessMessage()
{
	message = "";
	sender = NULL;
}

ProcessMessage::ProcessMessage( MessageType::MESSAGE_TYPE p_type,
							   ThreadSafeMessaging* p_sender, string p_message )
{
	type = p_type;
	message = p_message;
	sender = p_sender;
}

ProcessMessage::~ProcessMessage()
{
}
