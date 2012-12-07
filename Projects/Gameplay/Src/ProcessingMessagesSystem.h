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

class ProcessingMessagesSystem: public EntitySystem
{
public:
	ProcessingMessagesSystem();
	~ProcessingMessagesSystem();

	void process();

	void initialize();

};