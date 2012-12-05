// =======================================================================================
//                                      XInputFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Input fetcher for xbox360 controllers.
///        
/// # XInputFetcher
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

//

#include "InputHelper.h"

#include <Windows.h>
#include <Xinput.h>

//#define _WIN32_WINNT 0x0601

class XInputFetcher
{
public:
	XInputFetcher();
	virtual ~XInputFetcher();

	virtual void update();
	virtual int getBtnState( int p_btn );
	virtual short getAnalog( int p_analog );
	virtual double getAnalogAsDouble( int p_analog );

private:
	static int s_btnMaskMap[InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS];

	short m_analogs[InputHelper::NUM_XBOX360_CONTROLLER_ANALOGS];
	int m_btns[InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS];
	XINPUT_STATE m_currentState;
};

