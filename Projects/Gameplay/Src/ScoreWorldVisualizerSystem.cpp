#include "ScoreWorldVisualizerSystem.h"
#include <DebugUtil.h>
#include <ToString.h>
#include "Transform.h"
#include "ParticleSystemsComponent.h"
#include "LoadMesh.h"
#include "LookAtEntity.h"
#include "DestroyOnParticlesDeath.h"

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
			//cameraUp = camTransform->getUp();
			cameraRot = camTransform->getRotation();
		}
	}

	p_data.transform = AglMatrix(p_data.transform.GetScale(),
		cameraRot,p_data.transform.GetTranslation());
	Transform* transform = new Transform( p_data.transform );
	effect->addComponent( ComponentType::Transform, transform );
	LookAtEntity* lookat = new LookAtEntity(cam->getIndex());
	effect->addComponent(lookat);


	ParticleSystemsComponent* particleEmitters = new ParticleSystemsComponent();

	effect->addComponent(ComponentType::DestroyOnParticlesDeath,
						 new DestroyOnParticlesDeath());

	// add an emitter for each number in data
	string scorestr = toString(p_data.score);
	unsigned int size = scorestr.size();

	float w = 616.0f;
	float h = 16.0f;
	float ww = 8.0f;
	int no = w/ww;
	int offset = 16;

	float offsetN=(offset*ww)/w;
	float pixel=1.0f/w;
	float hN=h/h;
	float wwN=ww/w;

	for (unsigned int i=0;i<size;i++)
	{
		float numberoffset = (float)i*0.1f;
		AglParticleSystem particleSystem;
		particleSystem.getHeaderPtr()->rasterizerMode = AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP;
		AglVector3 offset = AglVector3( 5.0f*(-(float)size+(float)i),-30*numberoffset,0.0f);
		particleSystem.setParticleSize(AglVector2(5.0f,5.0f));
		// offset.transform(cameraRot);
		particleSystem.setSpawnPoint(offset);
		particleSystem.setSpawnDirection(AglVector3::up());
		particleSystem.setSpawnFrequency(10.0f);
		particleSystem.setAlignmentType(AglParticleSystemHeader::SCREEN);
		// particleSystem.set
		particleSystem.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
		particleSystem.setParticleSpace( AglParticleSystemHeader::AglSpace_LOCAL );
		particleSystem.setSpawnType(AglParticleSystemHeader::ONCE);
		particleSystem.setParticleAge(0.5f+numberoffset);
		particleSystem.setSpawnSpeed(15.0f+60*numberoffset);
		particleSystem.setFadeInStop(0.15f+numberoffset);
		particleSystem.setFadeOutStart(0.18f+numberoffset);
		if (p_data.score<0)
			particleSystem.setColor(AglVector4(1.0f,0.0f,0.0f,0.0f));
		// Create an instruction for creation
		ParticleSystemInstruction particleInstruction;
		particleInstruction.textureFileName = "text.png";
		particleInstruction.particleSystem = particleSystem;
		int idx = 0;
		string character = scorestr.substr(i,1);
		// get the index for the character
		if (character=="-")
			idx=-3; // the minus sign lies before the numbers
		else
			idx = min(9,atoi(character.c_str()));
		// calculate rect for character
		particleInstruction.uvRect = AglVector4(offsetN+idx*wwN,0.0f,offsetN-pixel+wwN+idx*wwN,hN);
		// add instruction
		particleEmitters->addParticleSystemInstruction(particleInstruction);
	}
	
	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
	
	m_world->addEntity(effect);
	return effect;
}
