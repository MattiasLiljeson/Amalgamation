#pragma once

#include "InputHelper.h"

class Control;

// =======================================================================================
//                                      ControlSet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Struct used to group together Controls used to control the cursor. Used by the 
/// Cursor class.
///        
/// # ControlSet
/// Detailed description.....
/// Created on: 4-1-2013 
/// \author Mattias Liljeson
///---------------------------------------------------------------------------------------

struct ControlSet
{
	Control* m_primaryBtn;
	Control* m_secondaryBtn;

	Control* m_xNegative;
	Control* m_xPositive;
	Control* m_yNegative;
	Control* m_yPositive;

	double m_xSensitivity;
	double m_ySensitivity;

	///-----------------------------------------------------------------------------------
	/// Returns the secondary buttons state
	/// \return InputHelper::KEY_STATE
	///-----------------------------------------------------------------------------------
	InputHelper::KEY_STATE getPrimaryState();

	///-----------------------------------------------------------------------------------
	/// Returns the secondary buttons state
	/// \return InputHelper::KEY_STATE
	///-----------------------------------------------------------------------------------
	InputHelper::KEY_STATE getSecondaryState();
	
	///-----------------------------------------------------------------------------------
	/// Help function. Converts status to State
	/// \param p_control
	/// \return InputHelper::KEY_STATE
	///-----------------------------------------------------------------------------------
	InputHelper::KEY_STATE getState( Control* p_control );

	///-----------------------------------------------------------------------------------
	/// returns the latest negative axis plus the positive axis times the sensitivity
	/// \return double
	///-----------------------------------------------------------------------------------
	double dx(double p_screenWidth);

	///-----------------------------------------------------------------------------------
	/// returns the latest negative axis plus the positive axis times the sensitivity
	/// \return double
	///-----------------------------------------------------------------------------------
	double dy(double p_screenHeight);

};

