#include "RocketLauncherModule.h"

ComponentRegister<RocketLauncherModule> RocketLauncherModule::s_reg("RocketLauncherModule");

RocketLauncherModule::RocketLauncherModule()
	: Component( ComponentType::RocketLauncherModule )
{
	lockCoolDown=0.0f; 
	laserSightEntity=-1;
	burstCount = 1;
	timeSinceRocket = FLT_MAX;
	currentBurst = 0;
	target = -1;
	lockOnPlaying = false;
}

RocketLauncherModule::RocketLauncherModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection)
	: Component( ComponentType::RocketLauncherModule )
{	
	muzzleOffset = p_muzzleOffset;
	fireDirection = p_fireDirection;
	laserSightEntity = -1;
	lockCoolDown = 0;
	cooldown = 0;

	burstCount = 1;
	timeSinceRocket = FLT_MAX;
	currentBurst = 0;
	target = -1;
	lockOnPlaying = false;
}

RocketLauncherModule::~RocketLauncherModule()
{
}

void RocketLauncherModule::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		// "Muzzle offset"
		if(p_initData[i].dataName == "offsetX")
			p_initData[i].getData<float>(&muzzleOffset.x);
		else if(p_initData[i].dataName == "offsetY")
			p_initData[i].getData<float>(&muzzleOffset.y);
		else if(p_initData[i].dataName == "offsetZ")
			p_initData[i].getData<float>(&muzzleOffset.z);
		// Fire direction
		else if(p_initData[i].dataName == "fireDirectionX")
			p_initData[i].getData<float>(&fireDirection.x);
		else if(p_initData[i].dataName == "fireDirectionY")
			p_initData[i].getData<float>(&fireDirection.y);
		else if(p_initData[i].dataName == "fireDirectionZ")
			p_initData[i].getData<float>(&fireDirection.z);
	}
}