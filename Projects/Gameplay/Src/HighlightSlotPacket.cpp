#include "HighlightSlotPacket.h"
#include "PacketType.h"

HighlightSlotPacket::HighlightSlotPacket()
{
	id = 0;
}

HighlightSlotPacket::~HighlightSlotPacket()
{

}

Packet HighlightSlotPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ModuleHighlightPacket));	
	packet << id;

	return packet;
}

void HighlightSlotPacket::unpack( Packet& p_packet )
{
	p_packet >> id;
}
