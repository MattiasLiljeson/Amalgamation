#include "ProcessMessageTerminate.h"


ProcessMessageTerminate::ProcessMessageTerminate()
	: ProcessMessage( MessageType::TERMINATE, NULL )
{
}

ProcessMessageTerminate::ProcessMessageTerminate( ThreadSafeMessaging* p_sender )
	: ProcessMessage( MessageType::TERMINATE, p_sender )
{
}

ProcessMessageTerminate::~ProcessMessageTerminate()
{
}
