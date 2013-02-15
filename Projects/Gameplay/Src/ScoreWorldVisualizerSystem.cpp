#include "ScoreWorldVisualizerSystem.h"
#include <DebugUtil.h>
#include <ToString.h>
#include "Transform.h"
#include "ParticleSystemsComponent.h"
#include "LoadMesh.h"

ScoreWorldVisualizerSystem::ScoreWorldVisualizerSystem() : 
	EntitySystem( SystemType::ScoreWorldVisualizerSystem)
{

}

ScoreWorldVisualizerSystem::~ScoreWorldVisualizerSystem()
{

}

void ScoreWorldVisualizerSystem::initialize()
{

}

void ScoreWorldVisualizerSystem::process()
{
	while(!m_effectsToCreate.empty())
	{
		ScoreEffectCreationData data = m_effectsToCreate.back();
		// DEBUGWARNING((toString(data.score).c_str()));
		createNumberEffectEntity(data);
		m_effectsToCreate.pop_back();
	}
}

void ScoreWorldVisualizerSystem::addEffect( ScoreEffectCreationData& p_fx )
{
	m_effectsToCreate.push_back(p_fx);
}

Entity* ScoreWorldVisualizerSystem::createNumberEffectEntity( ScoreEffectCreationData& p_data )
{
	Entity* effect = m_world->createEntity();

 	Transform* transform = new Transform( p_data.transform );
 	effect->addComponent( ComponentType::Transform, transform );

	LoadMesh* lm = new LoadMesh( "P_sphere",true );
	effect->addComponent( ComponentType::LoadMesh, lm );
	
	ParticleSystemsComponent* particleEmitters = new ParticleSystemsComponent();
	// add an emitter for each number in data
	unsigned int size = toString(p_data.score).size();

	for (unsigned int i=0;i<size;i++)
	{
		AglParticleSystem particleSystem;
		particleSystem.setSpawnPoint(AglVector3((float)size/2.0f-(float)i,0.0f,0.0f));
		particleSystem.setSpawnAngularVelocity(1.0f);
		// May have to change this as particle system component is updated
		ParticleSystemInstruction particleInstruction;
		particleInstruction.textureFileName = "worldtexteffectfont.png";
		particleInstruction.particleSystem = particleSystem;
		// add instruction here instead later
		particleEmitters->addParticleSystemInstruction(particleInstruction);
	}
	
	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
	
	m_world->addEntity(effect);
	return effect;
}
