// =======================================================================================
//                                      ProcessMessageTerminate
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	ProcessMessage that says "Terminate".
///        
/// # ProcessMessageTerminate
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "ProcessMessage.h"

class ProcessMessageTerminate: public ProcessMessage
{
public:
	///-----------------------------------------------------------------------------------
	/// Create an empty terminate message.
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageTerminate();

	///-----------------------------------------------------------------------------------
	/// Create a terminate message.
	/// \param p_sender
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageTerminate( ThreadSafeMessaging* p_sender );

	virtual ~ProcessMessageTerminate();

};