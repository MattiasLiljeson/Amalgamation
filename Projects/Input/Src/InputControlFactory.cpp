#include "InputControlFactory.h"

InputControlFactory::InputControlFactory()
{

}

InputControlFactory::~InputControlFactory()
{

}

Control* InputControlFactory::create360controllerAnalog( 
	InputHelper::XBOX360_CONTROLLER_ANALOG p_axis, InputHelper::SUB_AXIS p_subAxis )
{
	Control* ctl = new AnalogueControl( p_axis, p_subAxis );
	return ctl;
}

Control* InputControlFactory::create360controllerDigital(
	InputHelper::XBOX360_CONTROLLER_DIGITAL p_btn )
{
	Control* ctl = new DigitalControl( p_btn );
	return ctl;
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

Control* InputControlFactory::createMouseMovement( InputHelper::MOUSE_AXIS p_axis,
											   InputHelper::SUB_AXIS p_subAxis )
{
	Control* ctl = new MouseMoveControl( p_axis, p_subAxis);
	return ctl;
}
