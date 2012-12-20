#pragma once
#include <EntitySystem.h>

class ThreadSafeMessaging;

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