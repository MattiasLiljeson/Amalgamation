#include "Input.h"

	//libRocketFromKeysMap[W]		= Rocket::Core::Input::KI_W;
	//libRocketFromKeysMap[A]		= Rocket::Core::Input::KI_A;
	//libRocketFromKeysMap[S]		= Rocket::Core::Input::KI_S;
	//libRocketFromKeysMap[D]		= Rocket::Core::Input::KI_D;
	//libRocketFromKeysMap[SPACE] = Rocket::Core::Input::KI_SPACE;
	//libRocketFromKeysMap[LCTRL] = Rocket::Core::Input::KI_LCONTROL;
	//libRocketFromKeysMap[F1]	= Rocket::Core::Input::KI_F1;
	//libRocketFromKeysMap[F2]	= Rocket::Core::Input::KI_F2;
	//libRocketFromKeysMap[F3]	= Rocket::Core::Input::KI_F3;
	//libRocketFromKeysMap[F4]	= Rocket::Core::Input::KI_F4;

int Input::calcState( int p_state, bool p_down )
{
	if( p_state == KEY_UP )
	{
		if( p_down )
			{ p_state = KEY_PRESSED; }
		else
			{ p_state = KEY_UP; }
	}
	else if( p_state == KEY_PRESSED )
	{
		if( p_down )
			{ p_state = KEY_DOWN; }
		else
			{ p_state = KEY_RELEASED; }
	}
	else if( p_state == KEY_DOWN )
	{
		if( p_down )
			{ p_state = KEY_DOWN; }
		else
			{ p_state = KEY_RELEASED; }
	}
	else if( p_state == KEY_RELEASED )
	{
		if( p_down )
			{ p_state = KEY_PRESSED; }
		else
			{ p_state = KEY_UP; }
	}
	else
	{
		// should NEVER happen!
		p_state = KEY_UP;
	}

	return p_state;
}

int Input::calcStateFromEvents( int p_state, bool p_pressed, bool p_released )
{
	if( p_pressed )
	{
		p_state = KEY_PRESSED;
	}
	else if ( p_released )
	{
		p_state = KEY_RELEASED;
	}
	else
	{
		if( p_state == KEY_PRESSED )
			p_state = KEY_DOWN;

		else if( p_state == KEY_RELEASED )
			p_state = KEY_UP;
	}

	return p_state;
}

//Rocket::Core::Input::KeyIdentifier Input::libRocketFromKeys( int m_key )
//{
//	return libRocketFromKeysMap[m_key];
//}
