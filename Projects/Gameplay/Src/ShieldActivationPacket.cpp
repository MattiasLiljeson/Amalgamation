#include "ShieldActivationPacket.h"

ShieldActivationPacket::ShieldActivationPacket()
{
	shieldActivationState = false;
	entityIndex = -1;
}

Packet ShieldActivationPacket::pack()
{
	Packet packet((char)PacketType::ShieldActivationPacket);
	packet << shieldActivationState;
	packet << entityIndex;
	return packet;
}

void ShieldActivationPacket::unpack( Packet& p_packet )
{
	p_packet >> shieldActivationState;
	p_packet >> entityIndex;
}