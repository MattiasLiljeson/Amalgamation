#include "EntityCreationPacket.h"
#include "PacketType.h"


EntityCreationPacket::EntityCreationPacket()
{
	entityType		= 'x';
	owner			= -1;
	playerID		= -1;
	networkIdentity = -1;
	translation		= AglVector3();
	scale			= AglVector3();
	rotation		= AglQuaternion();
	meshInfo		= -1;
	isLevelProp		= false;
	bsPos			= AglVector3(0, 0, 0);
	bsRadius		= -1;
}

EntityCreationPacket::~EntityCreationPacket()
{

}

Packet EntityCreationPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::EntityCreation));
	packet << entityType
		<< isLevelProp
		<< owner
		<< playerID
		<< networkIdentity
		<< meshInfo
		<< miscData
		<< translation
		<< rotation
		<< scale
		<< bsPos
		<< bsRadius;

	return packet;
}

void EntityCreationPacket::unpack( Packet& p_packet )
{
	p_packet >> entityType
		>> isLevelProp
		>> owner
		>> playerID
		>> networkIdentity
		>> meshInfo
		>> miscData
		>> translation
		>> rotation
		>> scale
		>> bsPos
		>> bsRadius;
}