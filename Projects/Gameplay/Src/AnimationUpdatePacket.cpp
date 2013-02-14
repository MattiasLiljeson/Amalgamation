#include "AnimationUpdatePacket.h"
#include "PacketType.h"

AnimationUpdatePacket::AnimationUpdatePacket()
{
	networkIdentity = -1;
}

Packet AnimationUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::AnimationUpdatePacket));	
	packet << networkIdentity;

	return packet;
}

void AnimationUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> networkIdentity;
}