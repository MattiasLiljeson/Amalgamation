#include "SelectionMarkerUpdatePacket.h"
#include "PacketType.h"

SelectionMarkerUpdatePacket::SelectionMarkerUpdatePacket()
{
	targetNetworkIdentity = -1;
	transform = AglMatrix::identityMatrix();
}

Packet SelectionMarkerUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SelectionMarkerUpdatePacket));	
	packet << targetNetworkIdentity
		   << transform;

	return packet;
}

void SelectionMarkerUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> targetNetworkIdentity
			 >> transform;
}