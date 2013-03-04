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

	void addDamage( const DamageAccumulator& p_dmg );
	void pushFrameDamageToBuffer();
	NumericBuffer<float, BUFFER_SIZE>* getBufferPtr();

	const int getParticleSystemIdx() const;
	void setParticleSystemIdx( const int p_idx );

private:
	int m_particleSystemIdx;
	float m_dmgMult;
	DamageAccumulator m_frameDmg;
	NumericBuffer<float, BUFFER_SIZE> m_dmgBuffer;

	static ComponentRegister<DamageComponent> s_reg;
};

