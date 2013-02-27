#include "AnomalyBomb.h"

ComponentRegister<AnomalyBomb> AnomalyBomb::s_reg("AnomalyBomb");

AnomalyBomb::AnomalyBomb()
	: Component(ComponentType::AnomalyBomb)
{
	lifeTime = 0.0f;
	explodeTime = 0.0f;
	radius = 0.0f;
	arriveRadius = 0.0f;
	eventHorizonRadius = 0.0f;
	impulse = 0.0f;
}

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

void AnomalyBomb::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "lifeTime") {
			p_initData[i] >> lifeTime;
		}
		else if(p_initData[i].dataName == "explodeTime") {
			p_initData[i] >> explodeTime;
		}
		else if(p_initData[i].dataName == "radius") {
			p_initData[i] >> radius;
		}
		else if(p_initData[i].dataName == "arriveRadius") {
			p_initData[i] >> arriveRadius;
		}
		else if(p_initData[i].dataName == "eventHorizonRadius") {
			p_initData[i] >> eventHorizonRadius;
		}
		else if(p_initData[i].dataName == "impulse") {
			p_initData[i] >> impulse;
		}
	}
}