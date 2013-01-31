#include "ProcessMessageReceivePacket.h"

ProcessMessageReceivePacket::ProcessMessageReceivePacket(
	ThreadSafeMessaging* p_sender, Packet p_packet )
	: ProcessMessage( MessageType::RECEIVE_PACKET, p_sender ), packet(p_packet)
{
}

ProcessMessageReceivePacket::~ProcessMessageReceivePacket()
{
}