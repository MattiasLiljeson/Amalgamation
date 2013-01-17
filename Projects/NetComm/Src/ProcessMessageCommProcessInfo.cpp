#include "ProcessMessageCommProcessInfo.h"

ProcessMessageCommProcessInfo::ProcessMessageCommProcessInfo(
	ThreadSafeMessaging* p_sender,
	unsigned int p_totalPacketsReceived, unsigned int p_totalPacketsSent )
	: ProcessMessage( MessageType::COMM_PROCESS_INFO, p_sender )
{
	totalPacketsReceived = p_totalPacketsReceived;
	totalPacketsSent = p_totalPacketsSent;
}
