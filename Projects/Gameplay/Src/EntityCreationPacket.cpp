#include "EntityCreationPacket.h"
#include "PacketType.h"


EntityCreationPacket::EntityCreationPacket()
{
	entityType		= 'x';
	owner			= -1;
	networkIdentity = -1;
	translation		= AglVector3();
	scale			= AglVector3();
	rotation		= AglQuaternion();
}

EntityCreationPacket::~EntityCreationPacket()
{

}

Packet EntityCreationPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::EntityCreation));
	packet << entityType
		<< owner
		<< networkIdentity
		<< translation
		<< rotation
		<< scale;

	return packet;
}

void EntityCreationPacket::unpack( Packet p_packet )
{
	p_packet >> entityType
		>> owner
		>> networkIdentity
		>> translation
		>> rotation
		>> scale;
}