#pragma once

#include "InputHelper.h"

#include <Windows.h>
#include <Xinput.h>

// =======================================================================================
//                                      XInputFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Input fetcher for xbox360 controllers.
///        
/// Uses the xInput API to fetch input from a xbox360 controller. At the time this class
/// only supports one controller. 
///
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class XInputFetcher
{
public:
	XInputFetcher();
	virtual ~XInputFetcher();

	virtual void update();
	virtual InputHelper::KeyStates getBtnState( int p_btn );
	virtual short getRawAnalog( int p_analog );

	///-----------------------------------------------------------------------------------
	/// Calibrated output. Use calibrate() to set calibration
	/// \param p_analog which analog stick/trigger to fetch input for
	/// \return double Between 0.0 and 1.0
	///-----------------------------------------------------------------------------------
	virtual double getCalibratedAnalog( int p_analog );
	virtual void calibrate( double p_epsilon );

private:
	static int s_btnMaskMap[InputHelper::Xbox360Digitals_CNT];

	int m_rawAnalogs[InputHelper::Xbox360Analogs_CNT];
	int m_analogOffsets[InputHelper::Xbox360Analogs_CNT];
	InputHelper::KeyStates m_btns[InputHelper::Xbox360Digitals_CNT];
	XINPUT_STATE m_currentState;

	 double m_epsilon;
};

