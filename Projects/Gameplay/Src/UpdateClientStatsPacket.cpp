#include "UpdateClientStatsPacket.h"

UpdateClientStatsPacket::UpdateClientStatsPacket()
{
	//ping = -1;
	activePlayers = 0;
	secondsUntilEndOfRound = 0;
	minutesUntilEndOfRound = 0;

	for (int i=0; i<MAXPLAYERS; i++)
	{
		playerIdentities[i] = -1;
		scores[i] = 0;
		ping[i] = -1;
	}
}

UpdateClientStatsPacket::~UpdateClientStatsPacket()
{

}

Packet UpdateClientStatsPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::UpdateClientStats));
	packet << activePlayers;
	packet << secondsUntilEndOfRound;
	packet << minutesUntilEndOfRound;
	packet << currentServerTimestamp;
	for (int i=0; i<MAXPLAYERS; i++)
	{
		packet << playerIdentities[i];
		packet << scores[i];
		packet << ping[i];
	}

	return packet;
}

void UpdateClientStatsPacket::unpack( Packet& p_packet )
{
	p_packet >> activePlayers;
	p_packet >> secondsUntilEndOfRound;
	p_packet >> minutesUntilEndOfRound;
	p_packet >> currentServerTimestamp;
	for (int i=0; i<MAXPLAYERS; i++)
	{
		p_packet >> playerIdentities[i];
		p_packet >> scores[i];
		p_packet >> ping[i];
	}
}
