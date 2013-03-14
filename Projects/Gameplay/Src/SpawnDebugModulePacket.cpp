#include "SpawnDebugModulePacket.h"

SpawnDebugModulePacket::SpawnDebugModulePacket()
{
	numberOfModules = 0;
}

Packet SpawnDebugModulePacket::pack()
{
	Packet packet((char)PacketType::SpawnDebugModulePacket);
	packet << shipPosition;
	packet << numberOfModules;
	for(unsigned char i=0; i<numberOfModules; i++)
	{
		packet << moduleTypes[i];
	}
	return packet;
}

void SpawnDebugModulePacket::unpack( Packet& p_packet )
{
	p_packet >> shipPosition;
	p_packet >> numberOfModules;
	for(unsigned char i=0; i<numberOfModules; i++)
	{
		p_packet >> moduleTypes[i];
	}
}