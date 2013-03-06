#include <Component.h>

#pragma once

// =======================================================================================
//                                      ModuleOnChamberStartPoint
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Component that is added on a module when it has been spawned using the spawn-
/// point system. It is removed when the module is attached on a ship. Once removed,
/// a tracking system can reset that spawn point to be "spawn available" again. 
///        
/// # ModuleOnChamberStartPoint
/// Detailed description.....
/// Created on: 5-3-2013 
///---------------------------------------------------------------------------------------

class ModuleOnChamberStartPoint : public Component
{
public:
	ModuleOnChamberStartPoint(int p_atChamber, int p_atSpawnPoint)
		: Component(ComponentType::ModuleOnChamberSpawnPoint)
	{
		atChamber		= p_atChamber;
		atSpawnPoint	= p_atSpawnPoint;
	}

	int atChamber;
	int atSpawnPoint;
};