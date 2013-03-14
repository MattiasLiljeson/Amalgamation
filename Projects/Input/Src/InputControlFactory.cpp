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

vector<pair<string, Control*>> InputControlFactory::createAll360Analogs()
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

	for( int i=0; i<InputHelper::Xbox360Analogs_CNT; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = create360Analog(
			(InputHelper::Xbox360Analogs)i );
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::create360Analog( 
	InputHelper::Xbox360Analogs p_axis )
{
	Control* ctl = new AnalogueControl( p_axis );
	return ctl;
}

vector<pair<string, Control*>> InputControlFactory::createAll360Digitals()
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

	for( int i=0; i<InputHelper::Xbox360Digitals_CNT; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = create360Digital(
			(InputHelper::Xbox360Digitals)i );
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::create360Digital(
	InputHelper::Xbox360Digitals p_btn )
{
	Control* ctl = new DigitalControl( p_btn );
	return ctl;
}

vector<pair<string, Control*>> InputControlFactory::createAToZ()
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
		ss << "KeyboardKeys_";
		ss << (char)( 'A' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_CHARS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KeyboardKeys)
			(InputHelper::KeyboardKeys_A + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}


vector<pair<string, Control*>> InputControlFactory::create0To9()
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
//		ss << "KEY_";
		ss << "KeyboardKeys_";
		ss << (char)( '0' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KeyboardKeys)
			(InputHelper::KeyboardKeys_0 + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}

vector<pair<string, Control*>> InputControlFactory::createNumpad0ToNumpad9()
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
		ss << "KeyboardKeys_NUMPAD_";
		ss << (char)( '0' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<NUM_NUMBERS; i++ )
	{
		nameAndControl.first = names[i];
		nameAndControl.second = createKeyboardKey((InputHelper::KeyboardKeys)
			(InputHelper::KeyboardKeys_NUMPAD_0 + i));
		controls.push_back( nameAndControl );
	}
	return controls;
}

Control* InputControlFactory::createKeyboardKey( InputHelper::KeyboardKeys p_key )
{
	Control* ctl = new KeyControl( p_key );
	return ctl;
}

vector< pair<string, Control*> > InputControlFactory::createAllMouseAxes()
{
	string names[InputHelper::MouseAxes_CNT];
	vector< pair< string, Control* > > controls;
	names[InputHelper::MouseAxes_X_POSITIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_X_POSITIVE";
	names[InputHelper::MouseAxes_X_NEGATIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_X_NEGATIVE";
	names[InputHelper::MouseAxes_Y_POSITIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_Y_POSITIVE";
	names[InputHelper::MouseAxes_Y_NEGATIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_Y_NEGATIVE";
	names[InputHelper::MouseAxes_Z_POSITIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_Z_POSITIVE";
	names[InputHelper::MouseAxes_Z_NEGATIVE - InputHelper::MouseAxes_FIRST] = "MouseAxes_Z_NEGATIVE";

	// Axes
//	for( int i=0; i<InputHelper::MouseAxes_CNT; i++ )
//	{
//		stringstream ss;
//		ss << "MouseAxes_";
//		ss << (char)( '0' + i );
//		names[i] = ss.str();
//	}

	for( int i=0; i<InputHelper::MouseAxes_CNT; i++ )
	{
		pair< string, Control* > nameAndControl;
		nameAndControl.first = names[i];
		InputHelper::MouseAxes axis = (InputHelper::MouseAxes)i;
		nameAndControl.second = createMouseAxis( axis );
		controls.push_back( nameAndControl );
	}

	return controls;
}

vector< pair<string, Control*> > InputControlFactory::createAllMouseButtons()
{
	string names[InputHelper::MouseButtons_CNT];
	vector< pair< string, Control* > > controls;

	// Buttons
	for( int i=0; i<InputHelper::MouseButtons_CNT; i++ )
	{
		stringstream ss;
		ss << "MouseButtons_";
		ss << (char)( '0' + i );
		names[i] = ss.str();
	}

	for( int i=0; i<InputHelper::MouseButtons_CNT; i++ )
	{
		pair< string, Control* > nameAndControl;
		nameAndControl.first = names[i];
		InputHelper::MouseButtons btn = (InputHelper::MouseButtons) i;
		nameAndControl.second = createMouseButton( btn );
		controls.push_back( nameAndControl );
	}

	return controls;
}

Control* InputControlFactory::createMouseButton( InputHelper::MouseButtons p_btn )
{
	Control* ctl = new MouseBtnControl( p_btn );
	return ctl;
}

Control* InputControlFactory::createMouseAxis( InputHelper::MouseAxes p_axis )
{
	Control* ctl = new MouseMoveControl( p_axis );
	return ctl;
}

// BELOW DOESN'T WORK
//========================================================================================
//Cursor* InputControlFactory::createMouseCursor()
//{
//	Cursor* cursor = new Cursor();
//	cursor->setControls( createMouseAxis(InputHelper::MouseAxes::X_NEGATIVE),
//		createMouseAxis(InputHelper::MouseAxes::X_POSITIVE),
//		createMouseAxis(InputHelper::MouseAxes::Y_NEGATIVE),
//		createMouseAxis(InputHelper::MouseAxes::Y_POSITIVE),
//		createMouseButton( InputHelper::MouseButtons::M_LBTN ),
//		createMouseButton( InputHelper::MouseButtons::M_RBTN ) );
//	return cursor;
//}
//
//Cursor* InputControlFactory::createGamepadCursor()
//{
//	Cursor* cursor = new Cursor();
//	cursor->setControls( 
//		create360Analog(InputHelper::Xbox360Analogs::THUMB_LX_NEGATIVE),
//		create360Analog(InputHelper::Xbox360Analogs::THUMB_LX_POSITIVE),
//		create360Analog(InputHelper::Xbox360Analogs::THUMB_LY_NEGATIVE),
//		create360Analog(InputHelper::Xbox360Analogs::THUMB_LY_POSITIVE),
//		create360Digital(InputHelper::Xbox360Digitals::BTN_A),
//		create360Digital(InputHelper::Xbox360Digitals::BTN_B) );
//	return cursor;
//}
//========================================================================================