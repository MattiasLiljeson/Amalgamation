#include "InputHelper.h"

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

// Gives performance warning, therefore commented away.
//InputHelper::KEY_STATE InputHelper::calcState( const InputHelper::KEY_STATE p_oldState,
//											  const int p_down )
//{
//	return calcState(p_oldState, (bool)p_down);
//}
InputHelper::KEY_STATE InputHelper::calcState( const InputHelper::KEY_STATE p_oldState,
											  const bool p_down )
{
	InputHelper::KEY_STATE state = KEY_UP;
	if( p_oldState == KEY_UP )
	{
		if( p_down )
			{ state = KEY_PRESSED; }
		else
			{ state = KEY_UP; }
	}
	else if( p_oldState == KEY_PRESSED )
	{
		if( p_down )
			{ state = KEY_DOWN; }
		else
			{ state = KEY_RELEASED; }
	}
	else if( p_oldState == KEY_DOWN )
	{
		if( p_down )
			{ state = KEY_DOWN; }
		else
			{ state = KEY_RELEASED; }
	}
	else if( p_oldState == KEY_RELEASED )
	{
		if( p_down )
			{ state = KEY_PRESSED; }
		else
			{ state = KEY_UP; }
	}
	else
	{
		// should NEVER happen!
		state = KEY_UP;
	}

	return state;
}

InputHelper::KEY_STATE InputHelper::calcStateFromEvents( 
	const InputHelper::KEY_STATE p_oldState, const bool p_pressed, const bool p_released )
{
	InputHelper::KEY_STATE state = KEY_UP;
	//static bool dbg = false;
	if( p_pressed )
	{
		state = KEY_PRESSED;
	}
	else if ( p_released )
	{
		state = KEY_RELEASED;
	}
	else
	{
		if( p_oldState == KEY_PRESSED )
		{
			state = KEY_DOWN;
			//dbg = true;
		}
		else if( p_oldState == KEY_RELEASED )
		{
			state = KEY_UP;
		}
		else if( p_oldState == KEY_UP )
		{
			// THIS SHOULD NEVER HAPPEN!
			state = KEY_UP;
		}
	}

	return state;
}

double InputHelper::statusDeltaFromState( const InputHelper::KEY_STATE p_state )
{
	double delta = 0.0;
	switch( p_state )
	{
	case InputHelper::KEY_DOWN:
		delta = 0.0;
		break;

	case InputHelper::KEY_PRESSED:
		delta = 1.0;
		break;

	case InputHelper::KEY_UP:
		delta = 0.0;
		break;

	case InputHelper::KEY_RELEASED:
		delta = -1.0;
		break;
	}

	return delta;
}

double InputHelper::statusFromState( const InputHelper::KEY_STATE p_state )
{
	double status = 0.0;
	switch( p_state )
	{
	case InputHelper::KEY_DOWN:
		status = 1.0;
		break;

	case InputHelper::KEY_PRESSED:
		status = 1.0;
		break;

	case InputHelper::KEY_UP:
		status = 0.0;
		break;

	case InputHelper::KEY_RELEASED:
		status = 0.0;
		break;
	}
	return status;
}

//Rocket::Core::Input::KeyIdentifier Input::libRocketFromKeys( int m_key )
//{
//	return libRocketFromKeysMap[m_key];
//}
