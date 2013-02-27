#include "AnomalyBomb.h"

AnomalyBomb::AnomalyBomb(float p_lifeTime, float p_explodeTime, float p_radius,
	float p_arriveRadius, float p_eventHorizonRadius, float p_impulse)
	: Component(ComponentType::AnomalyBomb)
{
	lifeTime = p_lifeTime;
	explodeTime = p_explodeTime;
	radius = p_radius;
	arriveRadius = p_arriveRadius;
	eventHorizonRadius = p_eventHorizonRadius;
	impulse = p_impulse;
}