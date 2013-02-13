#include "OnHitScoreEffectPacket.h"

OnHitScoreEffectPacket::OnHitScoreEffectPacket()
{

}

OnHitScoreEffectPacket::~OnHitScoreEffectPacket()
{

}


Packet OnHitScoreEffectPacket::pack()
{
	Packet packet( (char)PacketType::OnHitScoreEffectPacket );
	packet << position;
	packet << angle;
	packet << score;

	return packet;
}

void OnHitScoreEffectPacket::unpack( Packet& p_packet )
{
	p_packet >> position;
	p_packet >> angle;
	p_packet >> score;
}

