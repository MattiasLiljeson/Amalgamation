#pragma once

#include "InputType.h"
#include "AnalogueControl.h"
#include "DigitalControl.h"

class InputControlFactory
{
public:
	InputControlFactory();
	~InputControlFactory();

	// TODO: Move to Factory!
	/// Add a control by listening for output. Non blocking.
	/// @return -1  if no input has been detected. Id for the control if detected. 
	int addControlByListening();
	int addControlByType( InputType type, int subType );
};

