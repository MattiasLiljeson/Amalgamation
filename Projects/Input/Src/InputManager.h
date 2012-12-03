#pragma once

#include "AnalogueControl.h"
#include "DigitalControl.h"
#include "DirectInputFetcher.h"
#include "InputType.h"
#include "MessageLoopFetcher.h"
#include "XInputFetcher.h"

#include <vector>

class AnalogueControl;
//class Control;
class DigitalControl;
class DirectInputFetcher;
class InputType;
class MessageLoopFetcher;
class XInputFetcher;

using namespace std;

class InputManager
{
public:
	InputManager();
	~InputManager();

	/// @brief Adds a Control to the manager.
	/// The manager takes ownership of the control.
	/// @return An ID used when working with the Control through the handler.
	int addControl( Control* p_control );

	/// @brief getters used by controls to update themselves
	MessageLoopFetcher* getMessageLoopFetcher() { return m_mlFetcher; }
	XInputFetcher* getXInputFetcher() { return m_xifetcher; }

private:
	static const int DIGITAL_ID_OFFSET = INT_MAX/2;
	vector<Control*> m_controls;

	MessageLoopFetcher* m_mlFetcher;
	XInputFetcher* m_xifetcher;
};

