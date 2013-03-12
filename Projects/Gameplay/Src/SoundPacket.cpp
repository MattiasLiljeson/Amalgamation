#include "SoundPacket.h"
#include "PacketType.h"

SoundPacket::SoundPacket()
{
	queuedPlayingState = AudioHeader::PlayState::PLAY;
	soundType = AudioHeader::SoundType::AMBIENT;
	target = -1;
	targetSlot = -1;
}

Packet SoundPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SoundPacket));
	packet << queuedPlayingState
		<< soundType
		<< target
		<< targetSlot;

	return packet;
}

void SoundPacket::unpack( Packet& p_packet )
{
	p_packet	>> queuedPlayingState
		>> soundType
		>> target
		>> targetSlot;
}