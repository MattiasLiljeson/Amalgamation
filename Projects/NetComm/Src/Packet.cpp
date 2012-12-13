#include "Packet.h"

Packet::Packet()
{
	readPos = 1;
	m_data.push_back(0);
}

Packet::~Packet()
{

}

char* Packet::getDataPtr()
{
	if (! m_data.empty() )
	{
		m_data[0] = (char)m_data.size() - 1;
		return &m_data[0];
	}
	else
		return NULL;
}

unsigned int Packet::getDataSize() const
{
	return m_data.size();
}

bool Packet::isEmpty() const
{
	bool empty;
	empty = (m_data.size() <= 1);

	return empty;
}

void Packet::setData(char* p_data, unsigned int p_size)
{
	if (p_size <= 255)
	{
		m_data.resize(p_size);
		memcpy(&m_data[0], p_data, p_size);
	}
	else
		throw invalid_argument("Attempting to set data beyond the allowed data size (255)");
}

Packet& Packet::operator << (int p_data)
{	
	unsigned int dataSize = sizeof(p_data);

	if (m_data.size() + dataSize > 255)
	{
		throw std::out_of_range( "Trying to stream in more data than\
							what is allowed (255) to be written in the Packet." );
	}
	else 
	{
		unsigned int oldPacketSize = m_data.size();
		m_data.resize(m_data.size() + dataSize);
		memcpy(&m_data[oldPacketSize], &p_data, dataSize);
	}
	return *this;
}

Packet& Packet::operator >> (int& p_data)
{
	unsigned int dataSize = sizeof(p_data);

	if( m_data.size() - readPos < dataSize )
	{
		throw std::out_of_range( "Trying to stream out more data than\
							what is left to be read in the Packet." );
	}
	else
	{
		memcpy(&p_data, &m_data[readPos], dataSize); 
		readPos += dataSize;
	}
	return *this;
}
