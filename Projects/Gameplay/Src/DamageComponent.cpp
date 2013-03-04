#include "DamageComponent.h"

ComponentRegister<DamageComponent> DamageComponent::s_reg("DamageComponent");


DamageComponent::DamageComponent() : Component( ComponentType::DamageComponent )
{
	m_dmgMult = 1.0f;
	m_particleSystemIdx = -1;
}

DamageComponent::~DamageComponent()
{

}

void DamageComponent::addDamage( const DamageAccumulator& p_dmg )
{
	m_frameDmg += p_dmg;
}

void DamageComponent::pushFrameDamageToBuffer()
{
	m_dmgBuffer.addVal( m_dmgMult * m_frameDmg.accumulatedDamage );
	m_frameDmg.reset();
}

NumericBuffer<float, DamageComponent::BUFFER_SIZE>* DamageComponent::getBufferPtr()
{
	return &m_dmgBuffer;
}

const int DamageComponent::getParticleSystemIdx() const
{
	return m_particleSystemIdx;
}

void DamageComponent::setParticleSystemIdx( const int p_idx )
{
	m_particleSystemIdx = p_idx;
}
