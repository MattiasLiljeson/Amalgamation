// =======================================================================================
//                                      KeyControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Implementation of Control used for keyboard keys
///        
/// # KeyControl
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include "Control.h"
#include "Input.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"

class Input;
class InputManager;
class MessageLoopFetcher;

class KeyControl : public Control
{
public:
	KeyControl( Input::KEYBOARD_KEYS p_key );
	virtual ~KeyControl(void);

	virtual void update( InputManager* p_manager );

private:
	int m_key;
};

