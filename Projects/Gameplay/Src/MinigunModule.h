#pragma once

#include <Component.h>
#include <AglVector3.h>
#include <ComponentFactory.h>

struct MinigunRay
{
	AglVector3 p1; ///< Position 1
	AglVector3 p2; ///< Position 2
	float energy; ///< Energy of the ray. Loses some energy each hit

	//Temp
	int entityId;

	MinigunRay()
	{
		entityId = -1;
		energy = 0.0f;
	}
};

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
	MinigunModule();
	MinigunModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection);
	~MinigunModule();
	void init( vector<ComponentData> p_initData );

	AglVector3 muzzleOffset;
	AglVector3 fireDirection;
	float	   coolDown;

	//Array of rays representing the fire 
	static const int rayCount = 10;
	MinigunRay rays[rayCount];
	int currentRay;
	float range;

	//Maybe temporary
	int		   laserSightEntity;
	int		   particleSystemEntity;

	//Animation status
	bool animationPlaying;

	static ComponentRegister<MinigunModule> s_reg;

};