#include "UpdateClientStatsPacket.h"

UpdateClientStatsPacket::UpdateClientStatsPacket()
{
	ping = -1;
	score[0] = -1;
	score[1] = -1;
	score[2] = -1;
	score[3] = -1;
	score[4] = -1;
	score[5] = -1;
	score[6] = -1;
	score[7] = -1;
}

UpdateClientStatsPacket::~UpdateClientStatsPacket()
{

}

Packet UpdateClientStatsPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::UpdateClientStats));
	packet << ping;
	for (int i = 0; i < 8 ; i++)
	{
		packet << score[i];
	}

	return packet;
}

void UpdateClientStatsPacket::unpack( Packet p_packet )
{
	p_packet >> ping;
	for (int i = 0; i < 8 ; i++)
	{
		p_packet >> score[i];
	}
}
