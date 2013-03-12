#pragma once

#include "ProcessMessage.h"
class ThreadSafeMessaging;

// =======================================================================================
//	ProcessMessageAskForCommProcessInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Process message that only ASKS for CommunicationProcess info.
///        
/// # ProcessMessageAskForCommProcessInfo
/// It only contains a sender pointer so that it can respond with a message containing
/// the info itself (ProcessMessageCommProcessInfo).
/// Created on: 17-1-2013 
///---------------------------------------------------------------------------------------

class ProcessMessageAskForCommProcessInfo: public ProcessMessage
{
public:
	ProcessMessageAskForCommProcessInfo( ThreadSafeMessaging* p_sender );
	
};