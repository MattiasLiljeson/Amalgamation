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
		<< (char)additionalMisc.size();

	for (unsigned int misc = 0; misc < additionalMisc.size(); misc++)
	{
		packet << additionalMisc[misc];
	}

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
		>> scale;
	
	char miscSize;
	p_packet >> miscSize;
	if (miscSize > 0)
	{
		additionalMisc.resize(miscSize, 0);
		for (unsigned int misc = 0; misc < miscSize; misc++)
		{
			p_packet >> additionalMisc[misc];
		}
	}
}