// =======================================================================================
//                                      ProcessMessagesSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # ProcessMessagesSystem
/// Detailed description...
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <EntitySystem.h>
#include <ThreadSafeMessaging.h>

class ProcessingMessagesSystem: public EntitySystem
{
public:
	ProcessingMessagesSystem( ThreadSafeMessaging* p_messageProcesser );
	~ProcessingMessagesSystem();

	void process();

	void initialize();

private:
	ThreadSafeMessaging* m_messageProcesser;

};