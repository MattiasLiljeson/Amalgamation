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
	virtual InputHelper::KEY_STATE getBtnState( int p_btn );
	virtual short getRawAnalog( int p_analog );

	///-----------------------------------------------------------------------------------
	/// Calibrated output. Use calibrate() to set calibration
	/// \param p_analog which analog stick/trigger to fetch input for
	/// \return double Between 0.0 and 1.0
	///-----------------------------------------------------------------------------------
	virtual double getCalibratedAnalog( int p_analog );
	virtual void calibrate( double p_epsilon );

private:
	static int s_btnMaskMap[InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS];

	int m_rawAnalogs[InputHelper::NUM_XBOX360_CONTROLLER_ANALOGS];
	int m_analogOffsets[InputHelper::NUM_XBOX360_CONTROLLER_ANALOGS];
	InputHelper::KEY_STATE m_btns[InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS];
	XINPUT_STATE m_currentState;

	 double m_epsilon;
};

