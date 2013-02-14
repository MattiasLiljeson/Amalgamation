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
	ShieldPlate(float p_scaleSeed)
		: Component( ComponentType::ShieldPlate )
	{
		scaleSeed = p_scaleSeed;
		scale = p_scaleSeed;
		increasing = true;
	}

public:
	float scaleSeed;
	float scale;
	bool increasing;
	AglMatrix spawnTransform;

};