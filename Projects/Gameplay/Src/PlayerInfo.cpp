#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	playerName = "";
	playerID = -1;
}

PlayerInfo::~PlayerInfo()
{

}

Packet PlayerInfo::pack()
{
	Packet packet( (char)PacketType::PlayerInfo );
	packet << playerName << playerID;

	return packet;
}

void PlayerInfo::unpack( Packet& p_packet )
{
	p_packet >> playerName >> playerID;
}
