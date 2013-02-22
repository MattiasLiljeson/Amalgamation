#pragma once
#include <Component.h>
#include <ComponentFactory.h>
#include "ParticleSystemsComponent.h"

// =======================================================================================
//                         DestroyOnParticlesDeath
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	For destroying entities on particles expiration
///        
/// # DestroyOnParticlesDeath
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class DestroyOnParticlesDeath : public Component
{
public:
	DestroyOnParticlesDeath();
	virtual ~DestroyOnParticlesDeath();
	void init( vector<ComponentData> p_initData );
protected:
private:

	static ComponentRegister<DestroyOnParticlesDeath> s_reg;
};