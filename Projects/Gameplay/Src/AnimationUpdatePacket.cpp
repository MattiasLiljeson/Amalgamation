#include "AnimationUpdatePacket.h"
#include "PacketType.h"

AnimationUpdatePacket::AnimationUpdatePacket()
{
	networkIdentity = -1;
	shouldPlay = false;
}

Packet AnimationUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::AnimationUpdatePacket));	
	packet << networkIdentity
		   << shouldPlay;

	return packet;
}

void AnimationUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> networkIdentity
			 >> shouldPlay;
}