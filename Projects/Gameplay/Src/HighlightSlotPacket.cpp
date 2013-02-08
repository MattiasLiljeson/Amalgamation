#include "HighlightSlotPacket.h"
#include "PacketType.h"

HighlightSlotPacket::HighlightSlotPacket()
{
	id = 0;
	status = 1;
}

HighlightSlotPacket::HighlightSlotPacket( Mode p_mode,int p_slot/*=0*/ )
{
	switch(p_slot)
	{
	case UNHIGHLIGHT_ALL:
		id = -1;
		break;
	case UNHIGHLIGHT_SLOT:
		id = p_slot;
		status = 0;
		break;	
	case HIGHLIGHT_SLOT:
	default:
		id = p_slot;
		status = 1;
		break;
	}
}


HighlightSlotPacket::~HighlightSlotPacket()
{

}

Packet HighlightSlotPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ModuleHighlightPacket));	
	packet << id;
	packet << status;

	return packet;
}

void HighlightSlotPacket::unpack( Packet& p_packet )
{
	p_packet >> id;
	p_packet >> status;
}
