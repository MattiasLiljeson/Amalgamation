#include "PlayersWinLosePacket.h"

PlayersWinLosePacket::PlayersWinLosePacket()
{
	activePlayers = 0;
	for (int i=0; i<MAXPLAYERS; i++)
	{
		playerIdentities[i] = -1;
		scores[i] = 0;
		winner[i] = false;
	}
}

Packet PlayersWinLosePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::PlayerWinLose));
	packet << activePlayers;
	for(int i=0; i<MAXPLAYERS; i++)
	{
		packet << playerIdentities[i];
		packet << scores[i];
		packet << winner[i];
	}
	return packet;
}

void PlayersWinLosePacket::unpack( Packet& p_packet )
{
	p_packet >> activePlayers;
	for(int i=0; i<MAXPLAYERS; i++)
	{
		p_packet >> playerIdentities[i];
		p_packet >> scores[i];
		p_packet >> winner[i];
	}
}