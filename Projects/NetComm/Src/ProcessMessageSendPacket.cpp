#include "ProcessMessageSendPacket.h"

ProcessMessageSendPacket::ProcessMessageSendPacket()
	: ProcessMessage( MessageType::SEND_PACKET, NULL )
{

}

ProcessMessageSendPacket::ProcessMessageSendPacket(
	ThreadSafeMessaging* p_sender, Packet p_packet )
	: ProcessMessage( MessageType::SEND_PACKET, p_sender )
{
	packet = p_packet;
}

ProcessMessageSendPacket::~ProcessMessageSendPacket()
{
}