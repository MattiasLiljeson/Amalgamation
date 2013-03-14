#pragma once

#include <Component.h>
#include <AglVector3.h>
#include <ComponentFactory.h>

enum MinigunState
{
	IDLE, HIGHLIGHTED, FIRING
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

	float range;
	int		rayIndex;

	//Maybe temporary
	int		   laserSightEntity;
	int		   particleSystemEntity;

	float timeSinceSound;

	//Animation status
	bool animationPlaying;
	MinigunState  minigunState;

	static ComponentRegister<MinigunModule> s_reg;

};