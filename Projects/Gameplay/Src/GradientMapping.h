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
	AglVector4 playerSmall;
	AglVector4 playerBig;

	GradientMapping()
	{

	}

	GradientMapping(AglVector4 p_small, AglVector4 p_big){
		playerSmall = p_small;
		playerBig = p_big;
	}
};