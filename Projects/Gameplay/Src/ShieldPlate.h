#pragma once
#include <Component.h>
#include <AglMatrix.h>
// =======================================================================================
// ShieldPlate
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShieldPlate
/// Detailed description...
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------
class ShieldPlate: public Component
{
public:
	ShieldPlate(float p_scaleSeed, AglVector3 p_travelDir, float p_maxRange)
		: Component( ComponentType::ShieldPlate )
	{
		scaleSeed = p_scaleSeed;
		scale = 0.0f;
		increasing = true;
		travelDir = p_travelDir;
		maxRange = p_maxRange;
	}

public:
	float scaleSeed;
	float scale;
	bool increasing;
	AglMatrix spawnTransform;
	AglVector3 travelDir;
	float maxRange;

};