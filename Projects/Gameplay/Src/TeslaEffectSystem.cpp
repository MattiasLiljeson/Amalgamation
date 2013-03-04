#include "TeslaEffectSystem.h"
#include "Transform.h"
#include "LoadMesh.h"
#include "TeslaEffectPiece.h"
#include "NetsyncDirectMapperSystem.h"

TeslaEffectSystem::TeslaEffectSystem()
	: EntitySystem(SystemType::TeslaEffectSystem, 2, ComponentType::Transform,
	ComponentType::TeslaEffectPiece)
{
}

void TeslaEffectSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		TeslaEffectPiece* effectPiece = static_cast<TeslaEffectPiece*>(p_entities[i]->
			getComponent(ComponentType::TeslaEffectPiece));
		effectPiece->lifeTime -= m_world->getDelta();
		if(effectPiece->lifeTime <= 0.0f)
		{
			m_world->deleteEntity(p_entities[i]);
		}
	}
}

void TeslaEffectSystem::animateHits( int p_fromEntity, int* p_identitiesHit, int p_numberOfHits )
{
	for(int i=0; i<p_numberOfHits; i++)
	{
		animateHit(p_fromEntity, p_identitiesHit[i]);
	}
}

void TeslaEffectSystem::animateHit( int p_fromEntity, int p_toEntity )
{
	NetsyncDirectMapperSystem* netsyncMapper = static_cast<NetsyncDirectMapperSystem*>(
		m_world->getSystem(SystemType::NetsyncDirectMapperSystem));
	Entity* source = netsyncMapper->getEntity(p_fromEntity);
	Entity* target = netsyncMapper->getEntity(p_toEntity);
	if(source != NULL && target != NULL)
	{
		Transform* sourceTransform = static_cast<Transform*>(source->getComponent(
			ComponentType::Transform));
		Transform* targetTransform = static_cast<Transform*>(target->getComponent(
			ComponentType::Transform));
		animate(sourceTransform->getTranslation(), targetTransform->getTranslation());
	}
}

void TeslaEffectSystem::animate( const AglVector3& p_sourcePosition,
	const AglVector3& p_targetPosition )
{
	Entity* effectCenter = m_world->createEntity();
	Transform* transform = new Transform((p_sourcePosition + p_targetPosition) / 2.0f,
		AglQuaternion::identity(), AglVector3::one());
	effectCenter->addComponent(transform);
	effectCenter->addComponent(new LoadMesh("RockA.agl"));
	effectCenter->addComponent(new TeslaEffectPiece(0.1f));
	m_world->addEntity(effectCenter);
}