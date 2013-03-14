#pragma once

#include <Component.h>
#include <AglVector3.h>
#include <vector>
#include <ComponentFactory.h>

// =======================================================================================
//	MineLayerModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that deploys mines
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class RocketLauncherModule: public Component
{
public:
	RocketLauncherModule();
	RocketLauncherModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection);
	~RocketLauncherModule();
	void init( vector<ComponentData> p_initData );

public:
	AglVector3 muzzleOffset;
	AglVector3 fireDirection;
	float	   lockCoolDown;
	float	   cooldown;
	int		   burstCount;
	float	   timeSinceRocket;
	int		   currentBurst;

	//Maybe temporary
	int		   laserSightEntity;
	int		   target;
	bool	   lockOnPlaying;

	static ComponentRegister<RocketLauncherModule> s_reg;
};