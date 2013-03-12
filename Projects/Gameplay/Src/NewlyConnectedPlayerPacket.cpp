#include "NewlyConnectedPlayerPacket.h"

NewlyConnectedPlayerPacket::NewlyConnectedPlayerPacket()
{
	networkID = -1;
	score = 0.0f;
	ping = 0;
	playerID = -1;
	playerName = "UNKNOWN";
}

Packet NewlyConnectedPlayerPacket::pack()
{
	Packet packet((char)PacketType::NewlyConnectedPlayerPacket);
	packet << playerID;
	packet << playerName;
	packet << networkID;
	packet << score;
	packet << ping;
	return packet;
}

void NewlyConnectedPlayerPacket::unpack( Packet& p_packet )
{
	p_packet >> playerID;
	p_packet >> playerName;
	p_packet >> networkID;
	p_packet >> score;
	p_packet >> ping;
}
