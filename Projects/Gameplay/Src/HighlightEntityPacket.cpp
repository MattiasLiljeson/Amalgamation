#include "HighlightEntityPacket.h"

HighlightEntityPacket::HighlightEntityPacket()
{
	target = -1;
	on = false;
}

Packet HighlightEntityPacket::pack()
{
	Packet packet( (char)PacketType::HighlightEntityPacket );
	packet << target
		   << on;

	return packet;
}

void HighlightEntityPacket::unpack( Packet& p_packet )
{
	p_packet >> target
			 >> on;
}