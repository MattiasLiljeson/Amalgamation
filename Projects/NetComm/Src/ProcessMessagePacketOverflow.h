#pragma once

#include "ProcessMessage.h"

// =======================================================================================
//	ProcessMessagePacketOverflow
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # ProcessMessagePacketOverflow
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ProcessMessagePacketOverflow: public ProcessMessage
{
public:
	ProcessMessagePacketOverflow( unsigned int p_numberOfOverflowPackets,
		ThreadSafeMessaging* p_sender );

public:
	unsigned int numberOfOverflowPackets;

};