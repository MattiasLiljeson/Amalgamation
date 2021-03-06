#pragma once
#include "InputHelper.h"

// =======================================================================================
//                                      IMouseKeyboardFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # IMouseKeyboardFetcher
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------

class IMouseKeyboardFetcher
{
public:
	IMouseKeyboardFetcher();
	virtual ~IMouseKeyboardFetcher();

	virtual void update() = 0;
	virtual InputHelper::KeyStates getKeyState( int p_key ) = 0;
	virtual InputHelper::KeyStates getMouseBtnState( int p_key ) = 0;
	virtual int getMousePos( int p_axis ) = 0;
	virtual int getMouseTravel( int p_axis ) = 0;
	virtual void setMouseSensitivity( const double p_sensitivity ) = 0;
	virtual const double getMouseSensitivity() const = 0;

};