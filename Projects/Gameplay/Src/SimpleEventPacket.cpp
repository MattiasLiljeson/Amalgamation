#include "SimpleEventPacket.h"

SimpleEventPacket::SimpleEventPacket()
{
	type = EVENT_NONE;
}

Packet SimpleEventPacket::pack()
{
	Packet packet( (char)PacketType::SimpleEvent );
	packet << type;

	return packet;
}

void SimpleEventPacket::unpack( Packet& p_packet )
{
	p_packet >> type;
}