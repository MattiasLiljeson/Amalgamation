#include "BombActivationPacket.h"

BombActivationPacket::BombActivationPacket()
{
	netsyncId = -1;
}

Packet BombActivationPacket::pack()
{
	Packet packet((char)PacketType::BombActivationPacket);
	packet << netsyncId;
	return packet;
}

void BombActivationPacket::unpack( Packet& p_packet )
{
	p_packet >> netsyncId;
}