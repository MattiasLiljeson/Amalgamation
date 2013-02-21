#include "DestroyOnParticlesDeath.h"

ComponentRegister<DestroyOnParticlesDeath> DestroyOnParticlesDeath::s_reg("DestroyOnParticlesDeath");

DestroyOnParticlesDeath::DestroyOnParticlesDeath() : Component( ComponentType::DestroyOnParticlesDeath)
{

}

DestroyOnParticlesDeath::~DestroyOnParticlesDeath()
{

}

void DestroyOnParticlesDeath::init( vector<ComponentData> p_initData )
{

}
