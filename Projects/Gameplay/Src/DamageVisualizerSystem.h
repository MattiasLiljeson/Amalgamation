#pragma once
#include <EntitySystem.h>
#include "GraphicsBackendSystem.h"
#include <ParticleSystemInstruction.h>
class DamageVisualizerSystem : public EntitySystem
{
public:
	DamageVisualizerSystem( GraphicsBackendSystem* p_gfxBackend );
	virtual ~DamageVisualizerSystem();
	virtual void processEntities( const vector<Entity*>& p_entities );

	ParticleSystemInstruction createParticleSystemInstruction();

private:
	GraphicsBackendSystem* m_gfxBackend;
};

