#include "PongPacket.h"
#include "PacketType.h"

PongPacket::PongPacket()
{
	clientTime = 0;
}

Packet PongPacket::pack()
{
	Packet packet( (char)PacketType::Pong );
	packet << clientTime;

	return packet;
}

void PongPacket::unpack( Packet p_packet )
{
	p_packet >> clientTime;
}
