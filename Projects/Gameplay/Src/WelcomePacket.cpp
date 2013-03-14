#include "WelcomePacket.h"

WelcomePacket::WelcomePacket()
{
	clientNetworkIdentity = -1;
	playerID = -1;
	serverName = "monki";
}

WelcomePacket::~WelcomePacket()
{

}

Packet WelcomePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::WelcomePacket));
	packet << clientNetworkIdentity << playerID << serverName;

	return packet;
}

void WelcomePacket::unpack( Packet& p_packet )
{
	p_packet >> clientNetworkIdentity >> playerID >> serverName;
}
