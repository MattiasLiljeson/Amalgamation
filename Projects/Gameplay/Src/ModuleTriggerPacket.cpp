#include "ModuleTriggerPacket.h"

ModuleTriggerPacket::ModuleTriggerPacket()
{
}

Packet ModuleTriggerPacket::pack()
{
	Packet packet((char)PacketType::ModuleTriggerPacket);
	packet << moduleNetsyncIdentity;
	packet << moduleTrigger;
	return packet;
}

void ModuleTriggerPacket::unpack( Packet& p_packet )
{
	p_packet >> moduleNetsyncIdentity;
	p_packet >> moduleTrigger;
}