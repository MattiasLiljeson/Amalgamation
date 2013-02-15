#pragma once
#include <Component.h>
#include "GradientMapping.h"
// =======================================================================================
//                                      GradientComponent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # GradientComponent
/// Detailed description.....
/// Created on: 15-2-2013 
///---------------------------------------------------------------------------------------

class GradientComponent : public Component
{
public:
	GradientComponent(AglVector4 p_small, AglVector4 p_big);
public:
	GradientMapping m_color;
};