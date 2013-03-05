#include "TeslaHitPacket.h"

TeslaHitPacket::TeslaHitPacket()
{

}

Packet TeslaHitPacket::pack()
{
	Packet packet((char)PacketType::TeslaHitPacket);
	packet << identitySource;
	packet << numberOfHits;
	for(unsigned char i=0; i<numberOfHits; i++)
	{
		packet << identitiesHit[i];
	}
	return packet;
}

void TeslaHitPacket::unpack( Packet& p_packet )
{
	p_packet >> identitySource;
	p_packet >> numberOfHits;
	for(unsigned char i=0; i<numberOfHits; i++)
	{
		p_packet >> identitiesHit[i];
	}
}