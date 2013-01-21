#pragma once

#include "ProcessMessage.h"
class ThreadSafeMessaging;

// =======================================================================================
//	ProcessMessageCommProcessInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # ProcessMessageCommProcessInfo
/// Detailed description...
/// Created on: 17-1-2013 
///---------------------------------------------------------------------------------------

class ProcessMessageCommProcessInfo: public ProcessMessage
{
public:
	ProcessMessageCommProcessInfo( ThreadSafeMessaging* p_sender,
		unsigned int p_totalPacketsReceived, unsigned int p_totalPacketsSent );
	
public:
	unsigned int totalPacketsReceived;
	unsigned int totalPacketsSent;
};