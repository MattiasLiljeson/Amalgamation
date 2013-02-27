#include "AnomalyBomb.h"

AnomalyBomb::AnomalyBomb(float p_lifeTime, float p_explodeTime, float p_radius,
	float p_eventHorizonRadius)
	: Component(ComponentType::AnomalyBomb)
{
	lifeTime = p_lifeTime;
	explodeTime = p_explodeTime;
	radius = p_radius;
	eventHorizonRadius = p_eventHorizonRadius;
}