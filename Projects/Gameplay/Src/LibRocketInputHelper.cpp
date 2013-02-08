#include "LibRocketInputHelper.h"
#include <cstring>

KeyIdentifier LibRocketInputHelper::rocketKeyCodeMapping[InputHelper::KeyboardKeys_CNT];

void LibRocketInputHelper::initialize()
{
	memset(&rocketKeyCodeMapping, KI_UNKNOWN, sizeof(rocketKeyCodeMapping));

	// Add more buttons here!
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_0] = KI_0; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_1] = KI_1; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_2] = KI_2; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_3] = KI_3; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_4] = KI_4; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_5] = KI_5; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_6] = KI_6; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_7] = KI_7; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_8] = KI_8; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_9] = KI_9;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_A] = KI_A;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_B] = KI_B;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_C] = KI_C;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_D] = KI_D;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_E] = KI_E;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_F] = KI_F;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_G] = KI_G;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_H] = KI_H;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_I] = KI_I;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_J] = KI_J;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_K] = KI_K;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_L] = KI_L;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_M] = KI_M;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_N] = KI_N;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_O] = KI_O;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_P] = KI_P;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_Q] = KI_Q;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_R] = KI_R;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_S] = KI_S;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_T] = KI_T;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_U] = KI_U;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_V] = KI_V;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_W] = KI_W;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_X] = KI_X;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_Y] = KI_Y;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_Z] = KI_Z;

	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_0] = KI_NUMPAD0;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_1] = KI_NUMPAD1;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_2] = KI_NUMPAD2;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_3] = KI_NUMPAD3;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_4] = KI_NUMPAD4;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_5] = KI_NUMPAD5;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_6] = KI_NUMPAD6;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_7] = KI_NUMPAD7;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_8] = KI_NUMPAD8;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_NUMPAD_9] = KI_NUMPAD9;

	rocketKeyCodeMapping[InputHelper::KeyboardKeys_PERIOD]		= KI_OEM_PERIOD;
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_BACKSPACE]	= KI_BACK;

}

bool LibRocketInputHelper::isKeyMapped( int p_key )
{
	return rocketKeyCodeMapping[p_key] != KI_UNKNOWN;
}

Rocket::Core::Input::KeyIdentifier LibRocketInputHelper::rocketKeyFromInputKey( 
	int p_key )
{
	return rocketKeyCodeMapping[p_key];
}
