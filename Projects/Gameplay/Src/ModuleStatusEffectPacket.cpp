#include "ModuleStatusEffectPacket.h"

ModuleStatusEffectPacket::ModuleStatusEffectPacket(StatusType p_statusType, Mode p_mode)
{
	m_statusType=(int)p_statusType;
	m_mode=(int)p_mode;
	m_moduleNetworkId=-1;
	m_value=0;
}

ModuleStatusEffectPacket::~ModuleStatusEffectPacket()
{

}

Packet ModuleStatusEffectPacket::pack()
{
	Packet packet( (char)PacketType::ModuleStatusEffectPacket );
	packet << m_statusType;
	packet << m_mode;
	packet << m_moduleNetworkId;
	packet << m_value;

	return packet;
}

void ModuleStatusEffectPacket::unpack( Packet& p_packet )
{
	p_packet >> m_statusType;
	p_packet >> m_mode;
	p_packet >> m_moduleNetworkId;
	p_packet >> m_value;
}
