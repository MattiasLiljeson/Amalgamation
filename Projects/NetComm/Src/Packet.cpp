#include "Packet.h"

Packet::Packet()
{
	m_message = "";
}

Packet::Packet( string p_message )
{
	m_message = p_message;
}

Packet::~Packet()
{

}

std::string Packet::getMessage()
{
	return m_message;
}
