#include "ModuleStateChangePacket.h"


ModuleStateChangePacket::ModuleStateChangePacket()
{
	affectedModule = -1;
	previousParent = -1;
	currentParrent = -1;
}

Packet ModuleStateChangePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ModuleStateChangePacket));
	packet << affectedModule << previousParent << currentParrent;

	return packet;
}

void ModuleStateChangePacket::unpack( Packet& p_packet )
{
	p_packet >> affectedModule >> previousParent >> currentParrent;
}
