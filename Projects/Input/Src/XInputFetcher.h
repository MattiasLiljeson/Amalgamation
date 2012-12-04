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

#include "input.h"

#include <Windows.h>
#include <Xinput.h>

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
	static int s_btnMaskMap[Input::NUM_XBOX360_CONTROLLER_DIGITALS];

	short m_analogs[Input::NUM_XBOX360_CONTROLLER_ANALOGS];
	int m_btns[Input::NUM_XBOX360_CONTROLLER_DIGITALS];
	XINPUT_STATE m_currentState;
};

