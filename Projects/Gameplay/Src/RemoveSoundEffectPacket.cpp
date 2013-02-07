#include "RemoveSoundEffectPacket.h"

RemoveSoundEffectPacket::RemoveSoundEffectPacket()
{
}

Packet RemoveSoundEffectPacket::pack()
{
	Packet packet((char)PacketType::RemoveSoundEffect);
	packet << attachedNetsyncIdentity;
	return packet;
}

void RemoveSoundEffectPacket::unpack( Packet& p_packet )
{
	p_packet >> attachedNetsyncIdentity;
}