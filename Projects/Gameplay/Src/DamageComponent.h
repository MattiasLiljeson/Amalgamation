#pragma once

#include <Component.h>
#include <DamageAccumulator.h>
#include <NumericBuffer.h>

class DamageComponent : public Component
{
public:
	static const int BUFFER_SIZE = 25;

public:
	DamageComponent();
	virtual ~DamageComponent();

	void pushFramePerValuesToBuffer();
	void addDamage( const DamageAccumulator& p_dmg );
	void addHit( const DamageAccumulator& p_hit );
	NumericBuffer<float, BUFFER_SIZE>* getDmgBufferPtr();
	NumericBuffer<float, BUFFER_SIZE>* getHitBufferPtr();

	const int getDmgParticleSystemIdx() const;
	void setDmgParticleSystemIdx( const int p_idx );

	const int getHitParticleSystemIdx() const;
	void setHitParticleSystemIdx( const int p_idx );

private:
	int m_dmgParticleSystemIdx;
	int m_hitParticleSystemIdx;

	float m_dmgMult;
	float m_hitMult;
	DamageAccumulator m_frameDmg;
	DamageAccumulator m_frameHit;
	NumericBuffer<float, BUFFER_SIZE> m_dmgBuffer;
	NumericBuffer<float, BUFFER_SIZE> m_hitBuffer;

	static ComponentRegister<DamageComponent> s_reg;
};

