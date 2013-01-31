#include "EntityDeletionPacket.h"
#include "EntityType.h"

EntityDeletionPacket::EntityDeletionPacket()
{
	networkIdentity = -1;
}

EntityDeletionPacket::~EntityDeletionPacket()
{

}

Packet EntityDeletionPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::EntityDeletion));
	packet << networkIdentity;
	return packet;
}

void EntityDeletionPacket::unpack( Packet p_packet )
{
	p_packet >> networkIdentity;
}

