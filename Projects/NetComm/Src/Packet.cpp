#include "Packet.h"

#include <string>
#include <stdexcept>
#include <climits>

Packet::Packet( const Packet& p_other )
	: m_data(p_other.m_data),
	m_senderId(p_other.m_senderId),
	m_readPos(p_other.m_readPos),
	m_packetSize(p_other.m_packetSize),
	m_packetType(p_other.m_packetType),
	m_uniquePacketIdentifier(p_other.m_uniquePacketIdentifier),
	m_dataSize(p_other.m_dataSize)
{
}

Packet::Packet( int p_senderId, char* p_data, unsigned int p_size )
{
	clear();
	m_senderId = p_senderId;
	setData(p_data,p_size);
}

Packet::Packet( char p_packetType )
{
	clear();
	m_packetType = p_packetType;
}

Packet::~Packet()
{

}

void Packet::clear()
{
	m_uniquePacketIdentifier = 0;
	m_senderId = -1;
	m_readPos = HEADER_SIZE;
	m_data.resize(PACKET_BUFFER_SIZE);
	m_data[0] = 0;
	m_dataSize = (unsigned int)HEADER_SIZE;
}

char* Packet::getDataPtr()
{
	if (! m_data.empty() )
	{
		m_packetSize = m_dataSize - 1;
		writeHeaderData(&m_data[0]);
		return &m_data[0];
	}
	else
		return NULL;
}

unsigned int Packet::getDataSize() const
{
	return m_dataSize;
}

void Packet::setData(char* p_data, unsigned int p_size)
{
	if (p_size <= 255)
	{
		m_dataSize = p_size;
		memcpy(&m_data[0], p_data, p_size);
		readHeaderData(&m_data[0]);
	}
	else
		throw invalid_argument("Attempting to set data beyond the allowed data size (255)");
}

bool Packet::isEmpty() const
{
	bool empty;
	empty = (m_dataSize <= HEADER_SIZE);

	return empty;
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

Packet& Packet::operator<<( unsigned short p_data )
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

Packet& Packet::operator << ( unsigned int p_data )
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

Packet& Packet::operator<<( AglMatrix& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}


Packet& Packet::operator << ( AglVector3& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator <<( AglQuaternion& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator<<( SYSTEMTIME p_data )
{
	unsigned int dataSize = sizeof(p_data);
	WriteData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator<<( string p_data )
{
	vector<unsigned char> buffer;
	buffer.resize(p_data.size() + 1);
	buffer[0] = static_cast<unsigned char>(p_data.size());
	memcpy(&buffer[1], p_data.c_str(), p_data.size());
	WriteData(buffer.data(), buffer.size());
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

Packet& Packet::operator>>( unsigned short& p_data )
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

Packet& Packet::operator>>( unsigned int& p_data )
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

Packet& Packet::operator >> ( AglMatrix& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator >> ( AglVector3& p_data)
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator>>( AglQuaternion& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator>>( SYSTEMTIME& p_data )
{
	unsigned int dataSize = sizeof(p_data);
	ReadData(&p_data, dataSize);
	return *this;
}

Packet& Packet::operator>>( string& p_data )
{
	unsigned char stringSize = 0;
	ReadData(&stringSize, 1); // Read first byte as a string size.
	p_data.resize(static_cast<unsigned int>(stringSize));
	ReadData(&p_data[0], static_cast<unsigned int>(stringSize));
	return *this;
}

void Packet::WriteData(void* p_data, unsigned int p_dataSize)
{
	if (m_dataSize + p_dataSize > 255)
	{
		throw std::out_of_range( "Trying to stream in more data than\
							what is allowed (255) to be written in the Packet." );
	}
	else 
	{
		unsigned int oldPacketSize = m_dataSize;
		m_dataSize += p_dataSize;
		memcpy(&m_data[oldPacketSize], p_data, p_dataSize);
	}
}

void Packet::ReadData(void* p_data, unsigned int p_dataSize)
{
	if( m_dataSize - m_readPos < p_dataSize )
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

int Packet::getSenderId() const
{
	return m_senderId;
}

void Packet::setSenderId( int p_senderId )
{
	m_senderId = p_senderId;
}

char Packet::getPacketType() const
{
	return m_packetType;
}

unsigned int Packet::getUniquePacketIdentifier() const
{
	return m_uniquePacketIdentifier;
}

void Packet::setUniquePacketIdentifier( unsigned int p_uniquePacketIdentifier )
{
	m_uniquePacketIdentifier = p_uniquePacketIdentifier;
}

void Packet::writeHeaderData( char* p_data )
{
	memcpy(&p_data[0], &m_packetSize, 1);
	memcpy(&p_data[1], &m_packetType, 1);
	memcpy(&p_data[2], &m_uniquePacketIdentifier, 4);
}

void Packet::readHeaderData( char* p_data )
{
	memcpy(&m_packetSize, &p_data[0], 1);
	memcpy(&m_packetType, &p_data[1], 1);
	memcpy(&m_uniquePacketIdentifier, &p_data[2], 4);
}

void Packet::setDataTest( char* p_data, unsigned int p_size )
{
	setData( p_data, p_size );
}
