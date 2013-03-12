#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <ComponentFactory.h>
// =======================================================================================
// TeslaEffectPiece
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief One piece of the Tesla coil effect.
/// 
/// # TeslaEffectPiece
/// Its forward scale is the length of the effect (ranges from the source to the target),
/// in the forward direction.
/// For instance (AglVector3::forward() * effectLength).
/// Created on: 2-3-2013 
///---------------------------------------------------------------------------------------
class TeslaEffectPiece: public Component
{
public:
	TeslaEffectPiece();
	TeslaEffectPiece(AglVector3 p_forwardScale);
	TeslaEffectPiece(float p_lifeTime, AglVector3 p_forwardScale);
	void init( vector<ComponentData> p_initData );

public:
	float lifeTime;
	float maxLifeTime;
	AglVector3 forwardScale;
	bool damages;

public:
	static ComponentRegister<TeslaEffectPiece> s_reg;

};