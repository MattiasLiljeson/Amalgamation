#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
// AnomalyBomb
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # AnomalyBomb
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class AnomalyBomb: public Component
{
public:
	AnomalyBomb();
	AnomalyBomb(float p_lifeTime, float p_explodeTime, float p_radius,
		float p_arriveRadius, float p_eventHorizonRadius, float p_impulse);
	void init( vector<ComponentData> p_initData ) final;

public:
	float lifetime;
	float maxLifetime;
	float explodeTime;
	float radius;
	float arriveRadius;
	float eventHorizonRadius;
	float impulse;
	bool activated;
	bool physicsEnabled;

public:
	static ComponentRegister<AnomalyBomb> s_reg;
};