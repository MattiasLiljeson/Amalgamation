#include "SpawnExplosionPacket.h"
#include "PacketType.h"

SpawnExplosionPacket::SpawnExplosionPacket()
{
	position = AglVector3();
}

Packet SpawnExplosionPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SpawnExplosionPacket));	
	packet << position;

	return packet;
}

void SpawnExplosionPacket::unpack( Packet& p_packet )
{
	p_packet >> position;
}