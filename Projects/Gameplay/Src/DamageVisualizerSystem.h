#pragma once
#include <EntitySystem.h>
#include <ParticleSystemInstruction.h>


class DamageVisualizerSystem : public EntitySystem
{
public:
	DamageVisualizerSystem();
	virtual ~DamageVisualizerSystem();
	virtual void processEntities( const vector<Entity*>& p_entities );

	AglParticleSystemHeader createFullScreenAdditiveQuad();
	ParticleSystemInstruction createDmgParticleSystemInstruction();
	ParticleSystemInstruction createHitParticleSystemInstruction();
};

