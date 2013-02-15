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

	LoadMesh* lm = new LoadMesh( "P_sphere",true );
	effect->addComponent( ComponentType::LoadMesh, lm );
	
	// get camera up, spawn direction
	AglVector3 cameraUp = AglVector3::up();
	AglQuaternion cameraRot = AglQuaternion::identity();
	auto entitymanager = m_world->getEntityManager();
	Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
	if (cam)
	{
		Transform* camTransform = static_cast<Transform*>(
			cam->getComponent( ComponentType::Transform ) );
		if (camTransform)
		{
			cameraUp = camTransform->getUp();
			cameraRot = camTransform->getRotation();
		}
	}

	p_data.transform = AglMatrix(p_data.transform.GetScale(),
		cameraRot,p_data.transform.GetTranslation());
	Transform* transform = new Transform( p_data.transform );
	effect->addComponent( ComponentType::Transform, transform );


	ParticleSystemsComponent* particleEmitters = new ParticleSystemsComponent();
	// add an emitter for each number in data
	unsigned int size = toString(p_data.score).size();

	for (unsigned int i=0;i<size;i++)
	{
		AglParticleSystem particleSystem;
		AglVector3 offset = AglVector3( 5.0f*((float)size/2.0f-(float)i),0.0f,0.0f);
		// offset.transform(cameraRot);
		particleSystem.setSpawnPoint(offset);
		particleSystem.setSpawnDirection(cameraUp);
		particleSystem.setSpawnFrequency(3.0f);
		particleSystem.setAlignmentType(AglParticleSystemHeader::VELOCITY);
		// particleSystem.set
		particleSystem.setSpace(AglParticleSystemHeader::AglSpace_LOCAL);
		particleSystem.getHeaderPtr()->relative=false;
		particleSystem.setSpawnSpeed(3.0f);
		// particleSystem.setSpawnAngularVelocity(1.0f);
		// Create an instruction for creation
		ParticleSystemInstruction particleInstruction;
		particleInstruction.textureFileName = "worldtexteffectfont.png";
		particleInstruction.particleSystem = particleSystem;
		particleInstruction.uvRect = AglVector4(0.0f,0.0f,0.5f,0.5f);
		// add instruction
		particleEmitters->addParticleSystemInstruction(particleInstruction);
	}
	
	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
	
	m_world->addEntity(effect);
	return effect;
}
