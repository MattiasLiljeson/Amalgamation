#include "EntityUpdatePacket.h"
#include "PacketType.h"

EntityUpdatePacket::EntityUpdatePacket()
{
	entityType = 'x';
	networkIdentity = -1;
	translation = AglVector3();
	rotation	= AglQuaternion();
	scale		= AglVector3();
	velocity	= AglVector3();
	angularVelocity = AglVector3();
}