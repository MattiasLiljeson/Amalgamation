// =======================================================================================
//                                      TcpServer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpServer
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	void startListening( int p_port );

	void stopListening();

	bool isListening();

	bool hasNewConnections();

private:
	bool m_isListening;

};