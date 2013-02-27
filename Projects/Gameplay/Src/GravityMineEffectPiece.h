#pragma once
#include <Component.h>
#include <AglVector3.h>

class GravityMineEffectPiece: public Component
{
public:
	GravityMineEffectPiece(float p_minRadius, float p_maxRadius, float p_maxLifeTime,
		float p_startFactor = 0.0f)
		: Component(ComponentType::GravityMineEffectPiece)
	{
		minRadius = p_minRadius;
		maxRadius = p_maxRadius;
		maxLifeTime = p_maxLifeTime;
		lifeTime = p_startFactor * p_maxLifeTime;
	}

public:
	AglVector3 offsetVector;
	float maxRadius;
	float minRadius;
	float lifeTime;
	float maxLifeTime;
};