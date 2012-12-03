#pragma once

#include "input.h"

#include <Windows.h>
#include <Xinput.h>

class XInputFetcher
{
public:
	XInputFetcher();
	~XInputFetcher();

	virtual void update();
	virtual int getBtnState( int p_btn );
	virtual short getAnalog( int p_analog );
	virtual float getAnalogAsFloat( int p_analog );

private:
	//static int s_analogMap[Input::NUM_XBOX360_CONTROLLER_ANALOGS];
	static int s_btnMaskMap[Input::NUM_XBOX360_CONTROLLER_DIGITALS];

	short m_analogs[Input::NUM_XBOX360_CONTROLLER_ANALOGS];
	int m_btns[Input::NUM_XBOX360_CONTROLLER_DIGITALS];
	XINPUT_STATE m_currentState;
};

