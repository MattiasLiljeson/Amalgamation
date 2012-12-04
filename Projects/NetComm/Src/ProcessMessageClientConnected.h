// =======================================================================================
//                                      ProcessMessageClientConnected
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	ProcessMessage that contains a new socket connection.
///        
/// # ProcessMessageClientConnected
/// Contains a boost::asio::ip::tcp::socket pointer.
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <boost/asio.hpp>
#include "ProcessMessage.h"

using namespace boost::asio::ip;

class ProcessMessageClientConnected: public ProcessMessage
{
public:
	///-----------------------------------------------------------------------------------
	/// Create an empty socket message.
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageClientConnected();

	///-----------------------------------------------------------------------------------
	/// Create a ClientConnected message containing a socket connection.
	/// \param p_sender
	/// \param p_socket
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageClientConnected( ThreadSafeMessaging* p_sender, tcp::socket* p_socket );

	virtual ~ProcessMessageClientConnected();

public:
	tcp::socket* socket;

};