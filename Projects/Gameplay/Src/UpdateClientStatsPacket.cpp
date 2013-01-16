#include "UpdateClientStatsPacket.h"

UpdateClientStatsPacket::UpdateClientStatsPacket()
{
	ping = -1;

	for (int i=0;i<MAXPLAYERS;i++)
	{
		scores[i]		= -1;
		networkIds[i]	= -1;
	}
}

UpdateClientStatsPacket::~UpdateClientStatsPacket()
{

}

Packet UpdateClientStatsPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::UpdateClientStats));
	packet << ping;
	for (int i = 0; i < MAXPLAYERS ; i++)
	{
		packet << scores[i];
		packet << networkIds[i];
	}

	return packet;
}

void UpdateClientStatsPacket::unpack( Packet p_packet )
{
	p_packet >> ping;
	for (int i = 0; i < MAXPLAYERS ; i++)
	{
		p_packet >> scores[i];
		p_packet >> networkIds[i];
	}
}
