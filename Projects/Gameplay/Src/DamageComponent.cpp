#include "DamageComponent.h"

ComponentRegister<DamageComponent> DamageComponent::s_reg("DamageComponent");


DamageComponent::DamageComponent() : Component( ComponentType::DamageComponent )
{
	m_dmgMult = 1.0f;
	m_hitMult = 1.0f;
	m_dmgParticleSystemIdx = -1;
	m_hitParticleSystemIdx = -1;

}

DamageComponent::~DamageComponent()
{

}

void DamageComponent::pushFramePerValuesToBuffer()
{
	m_dmgBuffer.addVal( m_dmgMult * m_frameDmg.accumulatedDamage );
	m_frameDmg.reset();

	m_hitBuffer.addVal( m_hitMult * m_frameHit.accumulatedDamage );
	m_frameHit.reset();
}

void DamageComponent::addDamage( const DamageAccumulator& p_dmg )
{
	m_frameDmg += p_dmg;
}

void DamageComponent::addHit( const DamageAccumulator& p_hit )
{
	m_frameHit += p_hit;
}

NumericBuffer<float, DamageComponent::BUFFER_SIZE>* DamageComponent::getDmgBufferPtr()
{
	return &m_dmgBuffer;
}

NumericBuffer<float, DamageComponent::BUFFER_SIZE>* DamageComponent::getHitBufferPtr()
{
	return &m_hitBuffer;
}

const int DamageComponent::getDmgParticleSystemIdx() const
{
	return m_dmgParticleSystemIdx;
}

void DamageComponent::setDmgParticleSystemIdx( const int p_idx )
{
	m_dmgParticleSystemIdx = p_idx;
}

const int DamageComponent::getHitParticleSystemIdx() const
{
	return m_hitParticleSystemIdx;
}

void DamageComponent::setHitParticleSystemIdx( const int p_idx )
{
	m_hitParticleSystemIdx = p_idx;
}