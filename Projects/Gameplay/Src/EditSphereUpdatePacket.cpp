#include "EditSphereUpdatePacket.h"
#include "PacketType.h"

EditSphereUpdatePacket::EditSphereUpdatePacket()
{
	m_offset = AglVector3::zero();
	m_radius = 0;
}

Packet EditSphereUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::EditSphereUpdatePacket));	
	packet << m_offset
		<< m_radius;

	return packet;
}

void EditSphereUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> m_offset
		>> m_radius;
}