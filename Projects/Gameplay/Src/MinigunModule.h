#pragma once

#include <Component.h>
#include <AglVector3.h>

// =======================================================================================
//	MinigunModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that fires like a minigun
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class MinigunModule: public Component
{
public:
	MinigunModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection);
	~MinigunModule();

	AglVector3 muzzleOffset;
	AglVector3 fireDirection;
	float	   coolDown;

	//Maybe temporary
	int		   laserSightEntity;

};