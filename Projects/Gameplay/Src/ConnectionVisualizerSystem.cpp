#include "ConnectionVisualizerSystem.h"
#include <DebugUtil.h>
#include <ToString.h>
#include "Transform.h"
#include "ParticleSystemsComponent.h"
#include "LoadMesh.h"
#include "MeshOffsetTransform.h"

ConnectionVisualizerSystem::ConnectionVisualizerSystem() : 
	EntitySystem( SystemType::ConnectionVisualizerSystem)
{

}

ConnectionVisualizerSystem::~ConnectionVisualizerSystem()
{

}

void ConnectionVisualizerSystem::initialize()
{

}

void ConnectionVisualizerSystem::process()
{
	while(!m_effectsToCreate.empty())
	{
		ConnectionEffectData data = m_effectsToCreate.back();
		if (data.disabled)
		{
			for (unsigned int i = 0; i < m_createdEffects.size(); i++)
			{
				if (m_createdEffects[i].parent == data.parent &&
					m_createdEffects[i].slot == data.slot)
				{
					m_createdEffects[i].disabled = true;
				}
			}
		}
		else
		{
			int toReplace = -1;
			for (unsigned int i = 0; i < m_createdEffects.size(); i++)
			{
				if (m_createdEffects[i].disabled)
				{
					toReplace = i;
					break;
				}
			}
			if (toReplace >= 0)
			{
				replaceConnectionEffectEntity(data, toReplace);
			}
			else
			{
				data.data = createConnectionEffectEntity(data);
				m_createdEffects.push_back(data);
			}
		}
		m_effectsToCreate.pop_back();
	}
	for (unsigned int i = 0; i < m_createdEffects.size(); i++)
	{
		Transform* parentTrans = static_cast<Transform*>(m_createdEffects[i].parent->getComponent(ComponentType::Transform));

		AglMatrix parentMatrix = parentTrans->getMatrix();

		AglMatrix chilMatrix = AglMatrix::createTranslationMatrix(m_createdEffects[i].offset);
		AglVector3 childForward = m_createdEffects[i].forward;
		chilMatrix *= parentMatrix;
		childForward.transformNormal(parentMatrix);

		ParticleSystemsComponent* psc = static_cast<ParticleSystemsComponent*>
							(m_createdEffects[i].data->getComponent(ComponentType::ParticleSystemsComponent));

		AglParticleSystem* ps = psc->getParticleSystemPtr(0);
		if (ps)
		{
			ps->setSpawnPoint(chilMatrix.GetTranslation()+childForward*0.55f);

			if (m_createdEffects[i].disabled)
				ps->setSpawnFrequency(0);
			else
				ps->setSpawnFrequency(3.0f);
		}
	}
}

void ConnectionVisualizerSystem::addEffect( ConnectionEffectData p_fx )
{
	m_effectsToCreate.push_back(p_fx);
}

Entity* ConnectionVisualizerSystem::createConnectionEffectEntity(ConnectionEffectData& p_data )
{
	Entity* effect = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	Transform* parentTrans = static_cast<Transform*>(p_data.parent->getComponent(ComponentType::Transform));

	AglMatrix parentMatrix = parentTrans->getMatrix();

	AglMatrix chilMatrix = AglMatrix::createTranslationMatrix(p_data.offset);
	chilMatrix *= parentMatrix;

	effect->addComponent( ComponentType::Transform, new Transform(chilMatrix));

	AglParticleSystem particleSystem;
	particleSystem.setSpawnPoint(chilMatrix.GetTranslation());
	particleSystem.setSpawnDirection(AglVector3(0, 1, 0));
	particleSystem.setSpawnFrequency(3.0f);
	particleSystem.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	particleSystem.setParticleSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	particleSystem.setSpawnSpeed(0.0f);
	particleSystem.setParticleSize(AglVector2(1.0f, 1.0f));
	particleSystem.setParticleAge(0.25f);
	particleSystem.setFadeOutStart(0.0f);

	ParticleSystemInstruction particleInstruction;
	particleInstruction.textureFileName = "addModule.png";
	particleInstruction.particleSystem = particleSystem;
	particleEmitter->addParticleSystemInstruction(particleInstruction);

	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(effect);
	return effect;
}
void ConnectionVisualizerSystem::replaceConnectionEffectEntity(ConnectionEffectData& p_data, int p_index)
{
	p_data.data = m_createdEffects[p_index].data;
	m_createdEffects[p_index] = p_data;
}
void ConnectionVisualizerSystem::disableAll()
{
	for (unsigned int i = 0; i < m_createdEffects.size(); i++)
	{
		m_createdEffects[i].disabled = true;
	}
}
