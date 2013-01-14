#include "UpdateClientStatsPacket.h"

UpdateClientStatsPacket::UpdateClientStatsPacket()
{
	ping = -1;

	for (int i=0;i<MAXPLAYERS;i++)
		score[i] = -1;
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
		packet << score[i];
	}

	return packet;
}

void UpdateClientStatsPacket::unpack( Packet p_packet )
{
	p_packet >> ping;
	for (int i = 0; i < MAXPLAYERS ; i++)
	{
		p_packet >> score[i];
	}
}
