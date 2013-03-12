#include "PlayerReadyPacket.h"

PlayerReadyPacket::PlayerReadyPacket()
{
	playerId = -1;
	ready = false;
}

PlayerReadyPacket::~PlayerReadyPacket()
{

}

Packet PlayerReadyPacket::pack()
{
	Packet packet((char)PacketType::PlayerReadyPacket);
	packet << playerId << ready;
	return packet;
}

void PlayerReadyPacket::unpack( Packet& p_packet )
{
	p_packet >> playerId >> ready;
}

