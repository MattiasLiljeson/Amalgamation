#include "Packet.h"

Packet::Packet()
{
	readPos = 0;
}

Packet::~Packet()
{

}

char* Packet::getDataPtr() const
{
	if (! m_data.empty() )
		return &m_data[0];
	else
		return NULL;
}

unsigned int Packet::getDataSize() const
{
	return m_data.size();
}

void Packet::setData(char* p_data, unsigned int p_size)
{

}

Packet& Packet::operator << (int p_data)
{
	unsigned int dataSize = sizeof(p_data);

	unsigned int oldPacketSize = m_data.size();
	m_data.resize(m_data.size() + dataSize);
	memcpy(&m_data[oldPacketSize], &p_data, dataSize);
	
	return *this;
}

Packet& Packet::operator >> (int& p_data)
{
	unsigned int dataSize = sizeof(p_data);

	memcpy(&p_data, &m_data[readPos], dataSize); 
	readPos += dataSize;

	return *this;
}
