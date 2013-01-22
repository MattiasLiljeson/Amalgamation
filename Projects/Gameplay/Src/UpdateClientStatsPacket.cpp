#include "UpdateClientStatsPacket.h"

UpdateClientStatsPacket::UpdateClientStatsPacket()
{
	ping = -1;

	for (int i=0; i<MAXPLAYERS; i++)
	{
		playerIdentities[i] = -1;
		scores[i] = 0;
	}
}

UpdateClientStatsPacket::~UpdateClientStatsPacket()
{

}

Packet UpdateClientStatsPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::UpdateClientStats));
	packet << ping;
	packet << currentServerTimestamp;
	for (int i=0; i<MAXPLAYERS; i++)
	{
		packet << playerIdentities[i];
		packet << scores[i];
	}

	return packet;
}

void UpdateClientStatsPacket::unpack( Packet p_packet )
{
	p_packet >> ping;
	p_packet >> currentServerTimestamp;
	for (int i=0; i<MAXPLAYERS; i++)
	{
		p_packet >> playerIdentities[i];
		p_packet >> scores[i];
	}
}
