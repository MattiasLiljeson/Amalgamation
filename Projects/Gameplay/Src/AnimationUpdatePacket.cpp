#include "AnimationUpdatePacket.h"
#include "PacketType.h"

AnimationUpdatePacket::AnimationUpdatePacket()
{
	networkIdentity = -1;
	shouldPlay = false;
	float playSpeed = 1.0f;
	take = "";
	shouldQueue = false;
}

Packet AnimationUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::AnimationUpdatePacket));	
	packet << networkIdentity
		   << shouldPlay
		   << playSpeed
		   << shouldQueue		   
		   << take;

	return packet;
}

void AnimationUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> networkIdentity
			 >> shouldPlay
			 >> playSpeed
			 >> shouldQueue
			 >> take;
}