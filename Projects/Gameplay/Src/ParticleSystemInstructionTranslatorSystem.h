#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;

class ParticleSystemInstructionTranslatorSystem : public EntitySystem
{
public:
	ParticleSystemInstructionTranslatorSystem( GraphicsBackendSystem* p_gfxSystem );
	virtual ~ParticleSystemInstructionTranslatorSystem();

	void processEntities( const vector<Entity*>& p_entities );

private:
	 GraphicsBackendSystem* m_gfxSystem;
};

