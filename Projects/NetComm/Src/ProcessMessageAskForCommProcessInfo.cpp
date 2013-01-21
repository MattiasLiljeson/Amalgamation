#include "ProcessMessageAskForCommProcessInfo.h"
#include "ThreadSafeMessaging.h"

ProcessMessageAskForCommProcessInfo::ProcessMessageAskForCommProcessInfo(
	ThreadSafeMessaging* p_sender )
	: ProcessMessage( MessageType::ASK_FOR_COMM_PROCESS_INFO, p_sender )
{
	// Only need the sender.
}
