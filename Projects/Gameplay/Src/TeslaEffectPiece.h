#pragma once
#include <Component.h>
#include <AglVector3.h>
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
	TeslaEffectPiece(float p_lifeTime, AglVector3 p_forwardScale);

public:
	float lifeTime;
	AglVector3 forwardScale;

};