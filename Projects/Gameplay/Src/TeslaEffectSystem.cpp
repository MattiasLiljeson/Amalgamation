#include "TeslaEffectSystem.h"
#include "Transform.h"
#include "LoadMesh.h"
#include "TeslaEffectPiece.h"
#include "NetsyncDirectMapperSystem.h"
#include <RandomUtil.h>

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
		else
		{
			Transform* pieceTransform = static_cast<Transform*>(p_entities[i]->
				getComponent(ComponentType::Transform));
			float scaleFactor = RandomUtil::randomInterval(1.0f, 5.0f);
			AglVector3 upScale = AglVector3::right() * scaleFactor;
			AglVector3 rightScale = AglVector3::forward() * scaleFactor;
			AglVector3 scale = effectPiece->forwardScale + upScale + rightScale;
			pieceTransform->setScale(scale);
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
	// up is in-game forward.
	AglVector3 forwardScale = AglVector3::up() *
		(p_sourcePosition - p_targetPosition).length();
	float scaleFactor = RandomUtil::randomInterval(1.0f, 5.0f);
	// right is in-game up.
	AglVector3 upScale = AglVector3::right() * scaleFactor;
	// forward is in-game right.
	AglVector3 rightScale = AglVector3::forward() * scaleFactor;
	AglVector3 scale = AglVector3::one() + forwardScale + upScale + rightScale;
	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(),
		p_targetPosition - p_sourcePosition);
	Transform* transform = new Transform(p_sourcePosition,
		rotation, scale);
	effectCenter->addComponent(transform);
	effectCenter->addComponent(new LoadMesh("LightningPart.agl"));
	effectCenter->addComponent(new TeslaEffectPiece(0.1f, forwardScale));
	m_world->addEntity(effectCenter);
}