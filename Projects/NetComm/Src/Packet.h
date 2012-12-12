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
using namespace std;

class Packet
{
public:
	Packet();
	~Packet();

	char* getDataPtr();
	unsigned int getDataSize() const;

	void setData(char* p_data, unsigned int p_size);

	Packet& operator << (int  p_data);
	Packet& operator >> (int& p_data);

private:

	int readPos;
	vector<char> m_data;
};