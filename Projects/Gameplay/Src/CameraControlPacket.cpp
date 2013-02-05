#include "CameraControlPacket.h"

CameraControlPacket::CameraControlPacket()
{
	movement=AglVector3();
	state = steeringState;
	entityId = -1;
}

CameraControlPacket::~CameraControlPacket()
{

}


Packet CameraControlPacket::pack()
{
	Packet packet( (char)PacketType::CameraControlPacket );

	packet << state;
	packet << movement;
	packet << entityId;

	return packet;
}

void CameraControlPacket::unpack( Packet& p_packet )
{
	p_packet >> state;
	p_packet >> movement;
	p_packet >> entityId;
}

