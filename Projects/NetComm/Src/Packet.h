// =======================================================================================
//                                      Packet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # Packet
/// Detailed description...
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <climits>

#include <AglVector3.h>

using namespace std;

class Packet
{
public:
	///-----------------------------------------------------------------------------------
	/// Creates a packet with sender id set to -1 (no sender).
	/// \return 
	///-----------------------------------------------------------------------------------
	Packet();

	///-----------------------------------------------------------------------------------
	/// Creates a packet with its sender id.
	/// \param p_senderId
	/// \return 
	///-----------------------------------------------------------------------------------
	Packet( int p_senderId );

	~Packet();

	/// Gets a pointer to the raw data that is generated by the packet content.
	char* getDataPtr();

	/// Returns the size of the raw data. The size is actually 1 byte more than
	/// the total sum of whatever is streamed in to the packet.
	/// For example, you stream in 3x integers (3*4 = 12 bytes) and the
	/// getDataSize() method will return 13.
	unsigned int getDataSize() const;

	/// Sets the raw data of the packet. It is possible to stream out so much data
	/// that equals to the byte size p_size - 1, since one byte is allocated to
	/// the packet size itself.
	void setData(char* p_data, unsigned int p_size);
	bool isEmpty() const;
	void clear();

	///-----------------------------------------------------------------------------------
	/// Returns the id of the sender.
	/// \return int
	///-----------------------------------------------------------------------------------
	int getSenderId();

	///-----------------------------------------------------------------------------------
	/// Sets the identity of the sender.
	/// \param p_senderId
	/// \return void
	///-----------------------------------------------------------------------------------
	void setSenderId( int p_senderId );

	Packet& operator << (bool	p_data);
	Packet& operator << (char	p_data);
	Packet& operator << (short	p_data);
	Packet& operator << (int	p_data);
	Packet& operator << (float	p_data);
	Packet& operator << (double p_data);
	Packet& operator << (AglVector3 p_data);
	
	Packet& operator >> (bool&	p_data);
	Packet& operator >> (char&	 p_data);
	Packet& operator >> (short&	 p_data);
	Packet& operator >> (int&	 p_data);
	Packet& operator >> (float&  p_data);
	Packet& operator >> (double& p_data);
	Packet& operator >> (AglVector3& p_data);

private:
	void WriteData(void* p_data, unsigned int p_dataSize);
	void ReadData(void* p_data, unsigned int p_dataSize);

private:
	int m_readPos;
	vector<char> m_data;

	int m_senderId;

};