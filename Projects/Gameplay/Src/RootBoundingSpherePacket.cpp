#include "RootBoundingSpherePacket.h"
#include "PacketType.h"

RootBoundingSpherePacket::RootBoundingSpherePacket()
{
	targetNetworkIdentity = -1;
	position = AglVector3::zero();
	radius = 0;
}

Packet RootBoundingSpherePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::RootBoundingSpherePacket));	
	packet << targetNetworkIdentity
		<< position
		<< radius;

	return packet;
}

void RootBoundingSpherePacket::unpack( Packet& p_packet )
{
	p_packet >> targetNetworkIdentity
		>> position
		>> radius;
}