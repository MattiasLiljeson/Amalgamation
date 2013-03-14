#include "SpawnDebugModulePacket.h"

SpawnDebugModulePacket::SpawnDebugModulePacket()
{
	moduleType = -1;
}

Packet SpawnDebugModulePacket::pack()
{
	Packet packet((char)PacketType::SpawnDebugModulePacket);
	packet << moduleType;
	return packet;
}

void SpawnDebugModulePacket::unpack( Packet& p_packet )
{
	p_packet >> moduleType;
}