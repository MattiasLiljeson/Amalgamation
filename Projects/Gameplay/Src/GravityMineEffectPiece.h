#pragma once
#include <Component.h>
#include <AglVector3.h>

class GravityMineEffectPiece: public Component
{
public:
	GravityMineEffectPiece(float p_radius, float p_maxLifeTime,
		float p_startFactor = 0.0f)
		: Component(ComponentType::GravityMineEffectPiece)
	{
		radius = p_radius;
		maxLifeTime = p_maxLifeTime;
		lifeTime = p_startFactor * p_maxLifeTime;
	}

public:
	AglVector3 offsetVector;
	float radius;
	float lifeTime;
	float maxLifeTime;
};