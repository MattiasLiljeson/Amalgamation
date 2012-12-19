// =======================================================================================
//	PacketTranslator
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Base class for translating a packet to data and data to a packet.
///        
/// # PacketTranslator
/// Detailed description...
/// Created on: 17-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class PacketTranslator
{
public:
	PacketTranslator();

	virtual ~PacketTranslator();

	virtual char* getBytes() = 0;
	virtual void setBytes(char* p_byteData, unsigned int p_bytesSize) = 0;

	unsigned int getBytesSize();
};