#include "SpawnExplosionPacket.h"
#include "PacketType.h"

SpawnExplosionPacket::SpawnExplosionPacket()
{
	position = AglVector3();
	source = ExplosionSource::ROCKET;
}

Packet SpawnExplosionPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SpawnExplosionPacket));	
	packet << position
		   << source;
	return packet;
}

void SpawnExplosionPacket::unpack( Packet& p_packet )
{
	p_packet >> position
			 >> source;
}