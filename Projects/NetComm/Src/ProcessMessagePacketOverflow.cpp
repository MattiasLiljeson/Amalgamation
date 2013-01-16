#include "ProcessMessagePacketOverflow.h"


ProcessMessagePacketOverflow::ProcessMessagePacketOverflow(
	unsigned int p_numberOfOverflowPackets, ThreadSafeMessaging* p_sender )
	: ProcessMessage( MessageType::PACKET_OVERFLOW, p_sender )
{
	numberOfOverflowPackets = p_numberOfOverflowPackets;
}
