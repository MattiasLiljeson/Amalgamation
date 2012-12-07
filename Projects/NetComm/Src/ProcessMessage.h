// =======================================================================================
//                                      ProcessMessage
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Placeholder for a usable message.
///        
/// # ProcessMessage
/// Detailed description.....
/// Created on: 29-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "ThreadSafeMessaging.h"
using namespace std;

class ThreadSafeMessaging;

///
/// Describes what type of message a ProcessMessage contains.
///
struct MessageType
{
	enum MESSAGE_TYPE{
		NONE = -1,
		TEXT,
		CLIENT_CONNECTED,
		SOCKET_DISCONNECTED,
		TERMINATE,
		RECEIVE_PACKET,
		SEND_PACKET,
	};
};

class ProcessMessage
{
public:
	///-----------------------------------------------------------------------------------
	/// Create a message with actual content.
	/// \param MessageType p_type
	/// \param ThreadSafeMessaging * p_sender
	/// \param string p_message
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessage( MessageType::MESSAGE_TYPE p_type, ThreadSafeMessaging* p_sender );

	virtual ~ProcessMessage();

public:
	MessageType::MESSAGE_TYPE type;
	ThreadSafeMessaging* sender;

};