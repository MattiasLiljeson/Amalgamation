#include "ShipMassBoosterUpdatePacket.h"
#include "PacketType.h"


ShipMassBoosterUpdatePacket::ShipMassBoosterUpdatePacket()
{
	mass=0;
	boosters=0;
}

ShipMassBoosterUpdatePacket::~ShipMassBoosterUpdatePacket()
{

}

Packet ShipMassBoosterUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ShipMassBoosterUpdatePacket));
	packet << mass;
	packet << boosters;

	return packet;
}

void ShipMassBoosterUpdatePacket::unpack( Packet& p_packet )
{
	p_packet >> mass;
	p_packet >> boosters;
}
