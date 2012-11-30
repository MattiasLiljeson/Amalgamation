#include "ProcessMessageText.h"

ProcessMessageText::ProcessMessageText()
	: ProcessMessage( MessageType::TEXT, NULL )
{
}

ProcessMessageText::ProcessMessageText( ThreadSafeMessaging* p_sender, string p_text )
	: ProcessMessage( MessageType::TEXT, p_sender )
{
	text = p_text;
}

ProcessMessageText::~ProcessMessageText()
{
}
