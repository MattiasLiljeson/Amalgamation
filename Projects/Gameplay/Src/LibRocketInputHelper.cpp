#include "LibRocketInputHelper.h"
#include <cstring>

KeyIdentifier LibRocketInputHelper::rocketKeyCodeMapping[InputHelper::KeyboardKeys_CNT];

void LibRocketInputHelper::initialize()
{
	memset(&rocketKeyCodeMapping, KI_UNKNOWN, sizeof(rocketKeyCodeMapping));

	rocketKeyCodeMapping[InputHelper::KeyboardKeys_0]			= KI_0; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_1]			= KI_1; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_2]			= KI_2; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_3]			= KI_3; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_4]			= KI_4; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_5]			= KI_5; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_6]			= KI_6; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_7]			= KI_7; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_8]			= KI_8; 
	rocketKeyCodeMapping[InputHelper::KeyboardKeys_9]			= KI_9; 
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
