// =======================================================================================
//                                      ProcessMessageText
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	ProcessMessage that contains text (string).
///        
/// # ProcessMessageText
/// Contains text.
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "ProcessMessage.h"

using namespace std;

class ProcessMessageText: public ProcessMessage
{
public:
	///-----------------------------------------------------------------------------------
	/// Create an empty text message.
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageText();

	///-----------------------------------------------------------------------------------
	/// Create a text message containing a type and a sender.
	/// \param ThreadSafeMessaging * p_sender
	/// \param string p_text
	/// \returns 
	///-----------------------------------------------------------------------------------
	ProcessMessageText( ThreadSafeMessaging* p_sender, string p_text );

	virtual ~ProcessMessageText();

public:
	string text;

};