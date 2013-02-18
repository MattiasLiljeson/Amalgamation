#pragma once
#include <AglVector4.h>

// =======================================================================================
//                                      Gradient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Gradient
/// Detailed description.....
/// Created on: 15-2-2013 
///---------------------------------------------------------------------------------------
struct GradientMapping 
{
	AglVector4 layerOne;
	AglVector4 layerTwo;

	GradientMapping()
	{

	}

	GradientMapping(AglVector4 p_layerOne, AglVector4 p_layerTwo){
		layerOne = p_layerOne;
		layerTwo = p_layerTwo;
	}
};