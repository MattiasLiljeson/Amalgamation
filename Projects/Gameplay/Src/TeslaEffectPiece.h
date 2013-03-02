#pragma once
#include <Component.h>
// =======================================================================================
// TeslaEffectPiece
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief One piece of the Tesla coil effect.
/// 
/// # TeslaEffectPiece
/// Detailed description...
/// Created on: 2-3-2013 
///---------------------------------------------------------------------------------------
class TeslaEffectPiece: public Component
{
public:
	TeslaEffectPiece();
	TeslaEffectPiece(float p_lifeTime);

public:
	float lifeTime;

};