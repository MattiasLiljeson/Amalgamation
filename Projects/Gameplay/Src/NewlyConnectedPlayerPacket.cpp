#include "NewlyConnectedPlayerPacket.h"

NewlyConnectedPlayerPacket::NewlyConnectedPlayerPacket()
{
	playerID = -1;
	playerName = "UNKNOWN";
}

Packet NewlyConnectedPlayerPacket::pack()
{
	Packet packet((char)PacketType::NewlyConnectedPlayerPacket);
	packet << playerID;
	packet << playerName;
	return packet;
}

void NewlyConnectedPlayerPacket::unpack( Packet& p_packet )
{
	p_packet >> playerID;
	p_packet >> playerName;
}
