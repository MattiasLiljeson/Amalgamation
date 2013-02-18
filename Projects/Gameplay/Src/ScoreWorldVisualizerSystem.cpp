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

// 	LoadMesh* lm = new LoadMesh( "P_sphere",true );
// 	effect->addComponent( ComponentType::LoadMesh, lm );
	
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
	string scorestr = toString(p_data.score);
	unsigned int size = scorestr.size();

	float w = 616.0f;
	float h = 8.0f;
	float ww = 8.0f;
	int no = w/ww;
	int offset = 16;

	float oN=offset/w;
	float hN=h/h;
	float wwN=ww/w;

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
		particleSystem.getHeaderPtr()->particleSpace=false;
		//particleSystem.setSpawnType(AglParticleSystemHeader::ONCE);
		particleSystem.setSpawnSpeed(3.0f);
		// Create an instruction for creation
		ParticleSystemInstruction particleInstruction;
		particleInstruction.textureFileName = "text.png";
		particleInstruction.particleSystem = particleSystem;
		unsigned int idx = min((unsigned int)9,(unsigned int)atoi(scorestr.substr(i,1).c_str()));
		particleInstruction.uvRect = AglVector4(oN+idx*wwN,0.0f,oN+wwN+idx*wwN,hN);
		// add instruction
		particleEmitters->addParticleSystemInstruction(particleInstruction);
	}
	
	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
	
	m_world->addEntity(effect);
	return effect;
}
