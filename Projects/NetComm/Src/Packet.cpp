#include "Packet.h"

Packet::Packet()
{
	clear();
}

Packet::Packet( int p_senderId )
{
	m_senderId = p_senderId;
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

bool Packet::isEmpty() const
{
	bool empty;
	empty = (m_data.size() <= 1);

	return empty;
}

void Packet::clear()
{
	m_readPos = 1;
	m_data.resize(1);
	m_data[0] = 0;
	m_senderId = -1;
}

Packet& Packet::operator << ( bool p_data )
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << (char p_data)
{	
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << (short p_data)
{	
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << (int p_data)
{	
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << ( float p_data )
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << (double p_data)
{	
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator << (AglVector3 p_data)
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator>>( bool& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> (char& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> (short& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> (int& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> ( float& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> ( double& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> (AglVector3& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}


void Packet::WriteData(void* p_data, unsigned int p_dataSize)
{
	if (m_data.size() + p_dataSize > 255)
	{
		throw std::out_of_range( "Trying to stream in more data than\
							what is allowed (255) to be written in the Packet." );
	}
	else 
	{
		unsigned int oldPacketSize = m_data.size();
		m_data.resize(m_data.size() + p_dataSize);
		memcpy(&m_data[oldPacketSize], p_data, p_dataSize);
	}
}

void Packet::ReadData(void* p_data, unsigned int p_dataSize)
{
	if( m_data.size() - m_readPos < p_dataSize )
	{
		throw std::out_of_range( "Trying to stream out more data than\
							what is left to be read in the Packet." );
	}
	else
	{
		memcpy(p_data, &m_data[m_readPos], p_dataSize); 
		m_readPos += p_dataSize;
	}
}

int Packet::getSenderId()
{
	return m_senderId;
}

void Packet::setSenderId( int p_senderId )
{
	m_senderId = p_senderId;
}
