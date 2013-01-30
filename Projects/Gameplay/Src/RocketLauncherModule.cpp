#include "RocketLauncherModule.h"

RocketLauncherModule::RocketLauncherModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection)
{	
	muzzleOffset = p_muzzleOffset;
	fireDirection = p_fireDirection;
	laserSightEntity = -1;
	coolDown = 0;

	burstCount = 1;
	timeSinceRocket = 0;
	currentBurst = 0;
}
RocketLauncherModule::~RocketLauncherModule()
{

}