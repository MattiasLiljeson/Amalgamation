#include "TeslaCoilModule.h"

TeslaCoilModule::TeslaCoilModule()
	: Component(ComponentType::TeslaCoilModule)
{
	range = 0.0f;
	optimalRange = 0.0f;
}

TeslaCoilModule::TeslaCoilModule( float p_range, float p_optimalRange,
	float p_cooldownTime )
	: Component(ComponentType::TeslaCoilModule)
{
	range = p_range;
	optimalRange = p_optimalRange;
	cooldownTime = p_cooldownTime;
	cooldown = 0.0f;
}