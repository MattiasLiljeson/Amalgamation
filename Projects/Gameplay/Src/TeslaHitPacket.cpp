#include "TeslaHitPacket.h"

TeslaHitPacket::TeslaHitPacket()
{

}

Packet TeslaHitPacket::pack()
{
	Packet packet((char)PacketType::TeslaHitPacket);
	
	return packet;
}

void TeslaHitPacket::unpack( Packet& p_packet )
{

}