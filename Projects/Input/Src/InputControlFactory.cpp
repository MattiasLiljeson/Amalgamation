#include "AnalogueControl.h"
#include "Control.h"
#include "Cursor.h"
#include "DigitalControl.h"
#include "InputControlFactory.h"
#include "KeyControl.h"
#include "MouseBtnControl.h"
#include "MouseMoveControl.h"
#include <sstream>

InputControlFactory::InputControlFactory()
{

}

InputControlFactory::~InputControlFactory()
{

}

vector<pair<string, Control*>> InputControlFactory::create360controllerAnalogAll()
{
	vector<pair<string, Control*>> controls;
	pair<string, Control*> nameAndControl;
	
	string names[] = 
	{
		"THUMB_LX_POSITIVE",
		"THUMB_LX_NEGATIVE",
		"THUMB_LY_POSITIVE",
		"THUMB_LY_NEGATIVE",
		"THUMB_RX_POSITIVE",
		"THUMB_RX_NEGATIVE",
		"THUMB_RY_POSITIVE",
		"THUMB_RY_NEGATIVE",
		"TRIGGER_L",
		"TRIGGER_R"
	};

	for( int i=0;
		i<InputHelper::XBOX360_CONTROLLER_ANALOG::NUM_XBOX360_CONTROLLER_ANALOGS;
		i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = create360controllerAnalog(
			(InputHelper::XBOX360_CONTROLLER_ANALOG)i );
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::create360controllerAnalog( 
	InputHelper::XBOX360_CONTROLLER_ANALOG p_axis )
{
	Control* ctl = new AnalogueControl( p_axis );
	return ctl;
}

vector<pair<string, Control*>> InputControlFactory::create360controllerDigitalAll()
{
	vector<pair<string, Control*>> controls;
	pair<string, Control*> nameAndControl;

	string names[] = 
	{
		"DPAD_UP",
		"DPAD_DOWN",
		"DPAD_LEFT",
		"DPAD_RIGHT",
		"BTN_START",
		"BTN_BACK",
		"THUMB_PRESS_L",
		"THUMB_PRESS_R",
		"SHOULDER_PRESS_L",
		"SHOULDER_PRESS_R",
		"BTN_A",
		"BTN_B",
		"BTN_X",
		"BTN_Y"
	};

	for( int i=0;
		i<InputHelper::XBOX360_CONTROLLER_DIGITAL::NUM_XBOX360_CONTROLLER_DIGITALS;
		i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = create360controllerDigital(
			(InputHelper::XBOX360_CONTROLLER_DIGITAL)i );
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::create360controllerDigital(
	InputHelper::XBOX360_CONTROLLER_DIGITAL p_btn )
{
	Control* ctl = new DigitalControl( p_btn );
	return ctl;
}

vector<pair<string, Control*>> InputControlFactory::createKeysAToZ()
{
	// 26 characters in the alphabet
	const int NUM_CHARS = 26;
	string names[NUM_CHARS];
	vector<pair<string, Control*>> controls;
	pair<string, Control*> nameAndControl;

	// Generate strings used as names;
	for( int i=0; i<NUM_CHARS; i++ )
	{
		stringstream ss;
		ss << "KEY_";
		ss << (char)( 'A' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_CHARS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KEYBOARD_KEY)
			(InputHelper::KEY_A + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}


vector<pair<string, Control*>> InputControlFactory::createKeysZeroToNine()
{
	// 10 numbers
	const int NUM_NUMBERS = 10;
	string names[NUM_NUMBERS];
	vector<pair<string, Control*>> controls;
	pair<string, Control*> nameAndControl;

	// Generate strings used as names;
	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		stringstream ss;
		ss << "KEY_";
		ss << (char)( '0' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KEYBOARD_KEY)
			(InputHelper::KEY_0 + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}

vector<pair<string, Control*>> InputControlFactory::createKeysNumZeroToNumNine()
{
	// 10 numbers
	const int NUM_NUMBERS = 10;
	string names[NUM_NUMBERS];
	vector<pair<string, Control*>> controls;
	pair<string, Control*> nameAndControl;

	// Generate strings used as names;
	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		stringstream ss;
		ss << "KEY_NUM";
		ss << (char)( '0' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KEYBOARD_KEY)
			(InputHelper::KEY_NUM0 + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::createKeyboardKey( InputHelper::KEYBOARD_KEY p_key )
{
	Control* ctl = new KeyControl( p_key );
	return ctl;
}

Control* InputControlFactory::createMouseButton( InputHelper::MOUSE_BTN p_btn )
{
	Control* ctl = new MouseBtnControl( p_btn );
	return ctl;
}

Control* InputControlFactory::createMouseMovement( InputHelper::MOUSE_AXIS p_axis )
{
	Control* ctl = new MouseMoveControl( p_axis );
	return ctl;
}

// BELOW DOESN'T WORK
//========================================================================================
//Cursor* InputControlFactory::createMouseCursor()
//{
//	Cursor* cursor = new Cursor();
//	cursor->setControls( createMouseMovement(InputHelper::MOUSE_AXIS::X_NEGATIVE),
//		createMouseMovement(InputHelper::MOUSE_AXIS::X_POSITIVE),
//		createMouseMovement(InputHelper::MOUSE_AXIS::Y_NEGATIVE),
//		createMouseMovement(InputHelper::MOUSE_AXIS::Y_POSITIVE),
//		createMouseButton( InputHelper::MOUSE_BTN::M_LBTN ),
//		createMouseButton( InputHelper::MOUSE_BTN::M_RBTN ) );
//	return cursor;
//}
//
//Cursor* InputControlFactory::createGamepadCursor()
//{
//	Cursor* cursor = new Cursor();
//	cursor->setControls( 
//		create360controllerAnalog(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX_NEGATIVE),
//		create360controllerAnalog(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX_POSITIVE),
//		create360controllerAnalog(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY_NEGATIVE),
//		create360controllerAnalog(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY_POSITIVE),
//		create360controllerDigital(InputHelper::XBOX360_CONTROLLER_DIGITAL::BTN_A),
//		create360controllerDigital(InputHelper::XBOX360_CONTROLLER_DIGITAL::BTN_B) );
//	return cursor;
//}
//========================================================================================