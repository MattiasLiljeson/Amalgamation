#include "DisconnectPacket.h"

DisconnectPacket::DisconnectPacket()
{
	clientNetworkIdentity = -1;
	playerID = -1;
}

DisconnectPacket::~DisconnectPacket()
{

}

Packet DisconnectPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ClientDisconnect));
	packet << clientNetworkIdentity << playerID;

	return packet;
}

void DisconnectPacket::unpack( Packet& p_packet )
{
	p_packet >> clientNetworkIdentity >> playerID;
}
