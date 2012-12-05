// =======================================================================================
//                                      DigitalControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for digital buttons on the xbox360 controller.
///        
/// # DigitalControl
/// Detailed description.....
/// Created on: 4-12-2012
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include "Control.h"
#include "InputHelper.h"
#include "InputManager.h"
#include "XInputFetcher.h"

class InputHelper;
class InputManager;
class XInputFetcher;

class DigitalControl : public Control
{
public:
	DigitalControl( InputHelper::XBOX360_CONTROLLER_DIGITALS p_btn );
	~DigitalControl();

	virtual void update( InputManager* p_manager );

private:
	InputHelper::XBOX360_CONTROLLER_DIGITALS m_btn;
};
