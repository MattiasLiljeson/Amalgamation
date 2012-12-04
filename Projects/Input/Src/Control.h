// =======================================================================================
//                                      Control
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	General class for all types of input (Keyboard, Mouse, GamePad). 
///        
/// # Control
/// Both analog (mouse, thumb sticks) and digital inputs (keys, buttons) use this class. 
/// A digital control will always cause the status to be 0 or 1. In an analog control 
/// the status spans between 0 and 1.0 depending on the amount of "pressness". The status 
/// delta is the difference of the status between the latest update and the one before
/// that. The delta can therefore also be negative.
///
/// Handles XBOX360 controllers and mouse and keyboard through the xInput API and the
/// message loop. When using the static message dispatch function in MessageLoopFetcher
/// must be called from the message loop. Otherwise no input from the keyboard and mouse
/// will be detected.
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

class InputManager;

class Control
{
public:
	Control();
	virtual ~Control();

	///@brief Override and update "pressness" member  variables.
	virtual void update(InputManager* p_manager) = 0;

	///@brief
	///@return The current status of the control. Between 0.0 and 1.0
	virtual double getStatus();

	///@brief
	///@return The delta of status from last update and the update before. 
	virtual double getDelta();

protected:
	// Current amount of "Pressness" on the control 
	double m_status;

	// "Pressness" delta between latest update and the update before.
	double m_statusDelta;
};

