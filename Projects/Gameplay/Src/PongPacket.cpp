#include "PongPacket.h"
#include "PacketType.h"

PongPacket::PongPacket()
{
	timeStamp = 0;
}

Packet PongPacket::pack()
{
	Packet packet( (char)PacketType::Pong );
	packet << timeStamp;

	return packet;
}

void PongPacket::unpack( Packet p_packet )
{
	p_packet >> timeStamp;
}
