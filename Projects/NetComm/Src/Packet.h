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

#include <string>
using namespace std;

class Packet
{
public:
	Packet();

	Packet( string p_message );

	~Packet();

	string getMessage();

private:
	string m_message;

};