#include "PingPacket.h"

PingPacket::PingPacket()
{
	timeStamp = 0;
}

Packet PingPacket::pack()
{
	Packet packet( (char)PacketType::Ping );
	packet << timeStamp;

	return packet;
}

void PingPacket::unpack( Packet& p_packet )
{
	p_packet >> timeStamp;
}