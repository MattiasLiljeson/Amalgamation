#include "EntityUpdatePacket.h"
#include "PacketType.h"

EntityUpdatePacket::EntityUpdatePacket()
{
	entityType = 'x';
	networkIdentity = -1;
	translation = AglVector3();
	rotation	= AglQuaternion();
	scale		= AglVector3();
}

EntityUpdatePacket::~EntityUpdatePacket()
{

}

Packet EntityUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::EntityUpdate));	
	packet << entityType
		<< networkIdentity
		<< translation
		<< rotation
		<< scale
		<< timestamp
		<< velocity
		<< angularVelocity;

	return packet;
}

void EntityUpdatePacket::unpack( Packet p_packet )
{
	p_packet >> entityType
		>> networkIdentity 
		>> translation 
		>> rotation 
		>> scale
		>> timestamp
		>> velocity
		>> angularVelocity;
}
