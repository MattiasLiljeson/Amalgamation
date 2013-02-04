#include "InputHelper.h"
#include <cstring>

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
//InputHelper::KeyStates InputHelper::calcState( const InputHelper::KeyStates p_oldState,
//											  const int p_down )
//{
//	return calcState(p_oldState, (bool)p_down);
//}

char InputHelper::lowerCaseCharacterMap[KeyboardKeys_CNT];
char InputHelper::upperCaseCharacterMap[KeyboardKeys_CNT];

InputHelper::KeyStates InputHelper::calcState( const InputHelper::KeyStates p_oldState,
											  const bool p_down )
{
	InputHelper::KeyStates state = KeyStates_KEY_UP;
	if( p_oldState == KeyStates_KEY_UP )
	{
		if( p_down )
			{ state = KeyStates_KEY_PRESSED; }
		else
			{ state = KeyStates_KEY_UP; }
	}
	else if( p_oldState == KeyStates_KEY_PRESSED )
	{
		if( p_down )
			{ state = KeyStates_KEY_DOWN; }
		else
			{ state = KeyStates_KEY_RELEASED; }
	}
	else if( p_oldState == KeyStates_KEY_DOWN )
	{
		if( p_down )
			{ state = KeyStates_KEY_DOWN; }
		else
			{ state = KeyStates_KEY_RELEASED; }
	}
	else if( p_oldState == KeyStates_KEY_RELEASED )
	{
		if( p_down )
			{ state = KeyStates_KEY_PRESSED; }
		else
			{ state = KeyStates_KEY_UP; }
	}
	else
	{
		// should NEVER happen!
		state = KeyStates_KEY_UP;
	}

	return state;
}

InputHelper::KeyStates InputHelper::calcStateFromEvents( 
	const InputHelper::KeyStates p_oldState, const bool p_pressed, const bool p_released )
{
	InputHelper::KeyStates state = KeyStates_KEY_UP;
	//static bool dbg = false;
	if( p_pressed )
	{
		state = KeyStates_KEY_PRESSED;
	}
	else if ( p_released )
	{
		state = KeyStates_KEY_RELEASED;
	}
	else
	{
		if( p_oldState == KeyStates_KEY_PRESSED )
		{
			state = KeyStates_KEY_DOWN;
			//dbg = true;
		}
		else if( p_oldState == KeyStates_KEY_RELEASED )
		{
			state = KeyStates_KEY_UP;
		}
		else if( p_oldState == KeyStates_KEY_UP )
		{
			// THIS SHOULD NEVER HAPPEN!
			state = KeyStates_KEY_UP;
		}
	}

	return state;
}

double InputHelper::statusDeltaFromState( const InputHelper::KeyStates p_state )
{
	double delta = 0.0;
	switch( p_state )
	{
	case InputHelper::KeyStates_KEY_DOWN:
		delta = 0.0;
		break;

	case InputHelper::KeyStates_KEY_PRESSED:
		delta = 1.0;
		break;

	case InputHelper::KeyStates_KEY_UP:
		delta = 0.0;
		break;

	case InputHelper::KeyStates_KEY_RELEASED:
		delta = -1.0;
		break;
	}

	return delta;
}

double InputHelper::statusFromState( const InputHelper::KeyStates p_state )
{
	double status = 0.0;
	switch( p_state )
	{
	case InputHelper::KeyStates_KEY_DOWN:
		status = 1.0;
		break;

	case InputHelper::KeyStates_KEY_PRESSED:
		status = 1.0;
		break;

	case InputHelper::KeyStates_KEY_UP:
		status = 0.0;
		break;

	case InputHelper::KeyStates_KEY_RELEASED:
		status = 0.0;
		break;
	}
	return status;
}

char InputHelper::charFromKeyboardKey( KeyboardKeys p_keys, bool p_uppercase/*=false*/ )
{
	if (p_uppercase)
		return upperCaseCharacterMap[p_keys];
	else
		return lowerCaseCharacterMap[p_keys];
}

void InputHelper::initialize()
{
	memset(&upperCaseCharacterMap, NONPRINTABLE_CHAR, sizeof(upperCaseCharacterMap));
	memset(&lowerCaseCharacterMap, NONPRINTABLE_CHAR, sizeof(lowerCaseCharacterMap));

	upperCaseCharacterMap[KeyboardKeys_A] = 'A';
	upperCaseCharacterMap[KeyboardKeys_B] = 'B';
	upperCaseCharacterMap[KeyboardKeys_C] = 'C';
	upperCaseCharacterMap[KeyboardKeys_D] = 'D';
	upperCaseCharacterMap[KeyboardKeys_E] = 'E';
	upperCaseCharacterMap[KeyboardKeys_F] = 'F';
	upperCaseCharacterMap[KeyboardKeys_G] = 'G';
	upperCaseCharacterMap[KeyboardKeys_H] = 'H';
	upperCaseCharacterMap[KeyboardKeys_I] = 'I';
	upperCaseCharacterMap[KeyboardKeys_J] = 'J';
	upperCaseCharacterMap[KeyboardKeys_K] = 'K';
	upperCaseCharacterMap[KeyboardKeys_L] = 'L';
	upperCaseCharacterMap[KeyboardKeys_M] = 'M';
	upperCaseCharacterMap[KeyboardKeys_N] = 'N';
	upperCaseCharacterMap[KeyboardKeys_O] = 'O';
	upperCaseCharacterMap[KeyboardKeys_P] = 'P';
	upperCaseCharacterMap[KeyboardKeys_Q] = 'Q';
	upperCaseCharacterMap[KeyboardKeys_R] = 'R';
	upperCaseCharacterMap[KeyboardKeys_S] = 'S';
	upperCaseCharacterMap[KeyboardKeys_T] = 'T';
	upperCaseCharacterMap[KeyboardKeys_U] = 'U';
	upperCaseCharacterMap[KeyboardKeys_V] = 'V';
	upperCaseCharacterMap[KeyboardKeys_W] = 'W';
	upperCaseCharacterMap[KeyboardKeys_X] = 'X';
	upperCaseCharacterMap[KeyboardKeys_Y] = 'Y';
	upperCaseCharacterMap[KeyboardKeys_Z] = 'Z';
	upperCaseCharacterMap[KeyboardKeys_0] = '0';
	upperCaseCharacterMap[KeyboardKeys_1] = '1';
	upperCaseCharacterMap[KeyboardKeys_2] = '2';
	upperCaseCharacterMap[KeyboardKeys_3] = '3';
	upperCaseCharacterMap[KeyboardKeys_4] = '4';
	upperCaseCharacterMap[KeyboardKeys_5] = '5';
	upperCaseCharacterMap[KeyboardKeys_6] = '6';
	upperCaseCharacterMap[KeyboardKeys_7] = '7';
	upperCaseCharacterMap[KeyboardKeys_8] = '8';
	upperCaseCharacterMap[KeyboardKeys_9] = '9';
	upperCaseCharacterMap[KeyboardKeys_PERIOD] = '.';

	lowerCaseCharacterMap[KeyboardKeys_A] = 'a';
	lowerCaseCharacterMap[KeyboardKeys_B] = 'b';
	lowerCaseCharacterMap[KeyboardKeys_C] = 'c';
	lowerCaseCharacterMap[KeyboardKeys_D] = 'd';
	lowerCaseCharacterMap[KeyboardKeys_E] = 'e';
	lowerCaseCharacterMap[KeyboardKeys_F] = 'f';
	lowerCaseCharacterMap[KeyboardKeys_G] = 'g';
	lowerCaseCharacterMap[KeyboardKeys_H] = 'h';
	lowerCaseCharacterMap[KeyboardKeys_I] = 'i';
	lowerCaseCharacterMap[KeyboardKeys_J] = 'j';
	lowerCaseCharacterMap[KeyboardKeys_K] = 'k';
	lowerCaseCharacterMap[KeyboardKeys_L] = 'l';
	lowerCaseCharacterMap[KeyboardKeys_M] = 'm';
	lowerCaseCharacterMap[KeyboardKeys_N] = 'n';
	lowerCaseCharacterMap[KeyboardKeys_O] = 'o';
	lowerCaseCharacterMap[KeyboardKeys_P] = 'p';
	lowerCaseCharacterMap[KeyboardKeys_Q] = 'q';
	lowerCaseCharacterMap[KeyboardKeys_R] = 'r';
	lowerCaseCharacterMap[KeyboardKeys_S] = 's';
	lowerCaseCharacterMap[KeyboardKeys_T] = 't';
	lowerCaseCharacterMap[KeyboardKeys_U] = 'u';
	lowerCaseCharacterMap[KeyboardKeys_V] = 'v';
	lowerCaseCharacterMap[KeyboardKeys_W] = 'w';
	lowerCaseCharacterMap[KeyboardKeys_X] = 'x';
	lowerCaseCharacterMap[KeyboardKeys_Y] = 'y';
	lowerCaseCharacterMap[KeyboardKeys_Z] = 'z';
	lowerCaseCharacterMap[KeyboardKeys_0] = '0';
	lowerCaseCharacterMap[KeyboardKeys_1] = '1';
	lowerCaseCharacterMap[KeyboardKeys_2] = '2';
	lowerCaseCharacterMap[KeyboardKeys_3] = '3';
	lowerCaseCharacterMap[KeyboardKeys_4] = '4';
	lowerCaseCharacterMap[KeyboardKeys_5] = '5';
	lowerCaseCharacterMap[KeyboardKeys_6] = '6';
	lowerCaseCharacterMap[KeyboardKeys_7] = '7';
	lowerCaseCharacterMap[KeyboardKeys_8] = '8';
	lowerCaseCharacterMap[KeyboardKeys_9] = '9';
	lowerCaseCharacterMap[KeyboardKeys_PERIOD] = '.';
}

//Rocket::Core::Input::KeyIdentifier InputHelper::libRocketFromKeys( int m_key )
//{
//	return libRocketFromKeysMap[m_key];
//}

//Rocket::Core::Input::KeyIdentifier InputHelper::libRocketFromKeysMap[KeyboardKeys_CNT];
