#include "ProcessMessageTerminate.h"


ProcessMessageTerminate::ProcessMessageTerminate()
	: ProcessMessage( MessageType::TERMINATE, 0 )
{
}

ProcessMessageTerminate::ProcessMessageTerminate( ThreadSafeMessaging* p_sender )
	: ProcessMessage( MessageType::TERMINATE, p_sender )
{
}

ProcessMessageTerminate::~ProcessMessageTerminate()
{
}
