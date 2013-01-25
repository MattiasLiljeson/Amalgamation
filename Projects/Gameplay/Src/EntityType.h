// =======================================================================================
//	EntityType
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkType
/// Detailed description...
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class EntityType
{
public:
	enum EntityEnums
	{
		NON_EXISTENT = -1, //Should never exist
		Ship,
		//Modules - Only add modules here
		ShipModuleStart, //Should never be used. Marks the start of module subsection
		MinigunModule,
		MineLayerModule, 
		ShieldModule,
		RocketLauncherModule,
		BoosterModule,
		EndModule, //Should never be used. Marks the end of module subsection
		//End Modules
		ParticleSystem,
		Mine,
		LaserSight, //For aiming
		Rocket,
		Shield,
		SelectionSphere, //For edit mode
		Other,
	};

public:
	EntityEnums type;
};