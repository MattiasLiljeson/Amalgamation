#include "ShieldModule.h"

ShieldModule::ShieldModule()
	: Component( ComponentType::ShieldModule )
{
	shieldAge = 0;
	cooldown = 0;
}