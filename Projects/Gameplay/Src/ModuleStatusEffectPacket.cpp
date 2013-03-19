#include "ModuleStatusEffectPacket.h"

ModuleStatusEffectPacket::ModuleStatusEffectPacket()
{
	m_statusType=0;
	m_mode=0;
	m_moduleNetworkId=-1;
	m_value=0;
}

ModuleStatusEffectPacket::ModuleStatusEffectPacket(StatusType p_statusType, Mode p_mode,
												   int p_networkId)
{
	m_statusType=(int)p_statusType;
	m_mode=(int)p_mode;
	m_moduleNetworkId=p_networkId;
	m_value=0;
}

ModuleStatusEffectPacket::ModuleStatusEffectPacket( StatusType p_statusType, 
												    float p_value,
												    int p_networkId )
{
	m_statusType=(int)p_statusType;
	m_mode=(int)Mode::ON;
	m_moduleNetworkId=p_networkId;
	m_value=p_value;
}

ModuleStatusEffectPacket::ModuleStatusEffectPacket( StatusType p_statusType, int p_networkId )
{
	m_statusType=(int)p_statusType;
	m_mode=(int)Mode::ON;
	m_moduleNetworkId=p_networkId;
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
