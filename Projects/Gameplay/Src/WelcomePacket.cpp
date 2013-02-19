#include "WelcomePacket.h"

WelcomePacket::WelcomePacket()
{
	clientNetworkIdentity = -1;
	playerID = -1;
}

WelcomePacket::~WelcomePacket()
{

}

Packet WelcomePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::WelcomePacket));
	packet << clientNetworkIdentity << playerID;

	return packet;
}

void WelcomePacket::unpack( Packet& p_packet )
{
	p_packet >> clientNetworkIdentity >> playerID;
}
