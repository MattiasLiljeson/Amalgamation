#pragma once

#include <Component.h>
#include <AglVector3.h>

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
	RocketLauncherModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection);
	~RocketLauncherModule();

	AglVector3 muzzleOffset;
	AglVector3 fireDirection;
	float	   coolDown;

	//Maybe temporary
	int		   laserSightEntity;

};