#include "ModulesHighlightPacket.h"

ModulesHighlightPacket::ModulesHighlightPacket()
{
	numberOfHighlights = 0;
}

void ModulesHighlightPacket::unpack( Packet& p_packet )
{
	p_packet >> numberOfHighlights;
	for(unsigned char i=0; i<numberOfHighlights; i++)
	{
		p_packet >> modulesHighighted[i];
	}
}

Packet ModulesHighlightPacket::pack()
{
	Packet packet((char)PacketType::ModulesHighlightPacket);
	packet << numberOfHighlights;
	for(unsigned char i=0; i<numberOfHighlights; i++)
	{
		packet << modulesHighighted[i];
	}
	return packet;
}