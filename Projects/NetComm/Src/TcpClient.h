// =======================================================================================
//                                      TcpClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpClient
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
using namespace std;

class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	void connectToServer( string p_address, int p_port );

};