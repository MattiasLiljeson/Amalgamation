#include "InputControlFactory.h"

InputControlFactory::InputControlFactory()
{

}

InputControlFactory::~InputControlFactory()
{

}

Control* InputControlFactory::add360controllerAnalog( 
	Input::XBOX360_CONTROLLER_ANALOGS p_axis, Input::SUB_AXIS p_subAxis )
{
	Control* ctl = new AnalogueControl( p_axis, p_subAxis );
	return ctl;
}

Control* InputControlFactory::add360controllerDigital(
	Input::XBOX360_CONTROLLER_DIGITALS p_btn )
{
	Control* ctl = new DigitalControl( p_btn );
	return ctl;
}

Control* InputControlFactory::addKeyboardKey( Input::KEYBOARD_KEYS p_key )
{
	Control* ctl = new KeyControl( p_key );
	return ctl;
}

Control* InputControlFactory::addMouseButton( Input::MOUSE_BTNS p_btn )
{
	Control* ctl = new MouseBtnControl( p_btn );
	return ctl;
}

Control* InputControlFactory::addMouseMovement( Input::MOUSE_AXIS p_axis,
											   Input::SUB_AXIS p_subAxis )
{
	Control* ctl = new MouseMoveControl( p_axis, p_subAxis);
	return ctl;
}
