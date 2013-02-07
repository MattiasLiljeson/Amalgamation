#pragma once

#include "InputHelper.h"
#include "Rocket/Core/Input.h"

using namespace Rocket::Core::Input;
// =======================================================================================
//                                      LibRocketInputHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief This class servers as a helper for librocket keyboard identifiers, by mapping
/// the InputHelper::KeyboardKeys to those. Lookup is O(1).
///        
/// # LibRocketInputHelper
/// Detailed description.....
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------

class LibRocketInputHelper
{
public:
	static void initialize();

	static bool			 isKeyMapped(int p_key);
	static KeyIdentifier rocketKeyFromInputKey(int p_key);
private:
	static KeyIdentifier rocketKeyCodeMapping[InputHelper::KeyboardKeys_CNT];
};