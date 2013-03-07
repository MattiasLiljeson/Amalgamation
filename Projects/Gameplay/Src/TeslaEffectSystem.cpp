#include "TeslaEffectSystem.h"
#include "Transform.h"
#include "LoadMesh.h"
#include "TeslaEffectPiece.h"
#include "NetsyncDirectMapperSystem.h"
#include <RandomUtil.h>
#include "EntityFactory.h"
#include "SpawnPointSet.h"
#include "MeshOffsetTransform.h"

TeslaEffectSystem::TeslaEffectSystem()
	: EntitySystem(SystemType::TeslaEffectSystem, 3, ComponentType::Transform,
	ComponentType::MeshOffsetTransform, ComponentType::SpawnPointSet)
{
}

void TeslaEffectSystem::animateHits( int p_fromEntity, int* p_identitiesHit, int p_numberOfHits )
{
	AglVector3 geometricMean = AglVector3::zero();
	NetsyncDirectMapperSystem* netsyncMapper = static_cast<NetsyncDirectMapperSystem*>(
		m_world->getSystem(SystemType::NetsyncDirectMapperSystem));
	Entity* entitySource = netsyncMapper->getEntity(p_fromEntity);
	if(entityInSystem(entitySource))
	{
		for(int i=0; i<p_numberOfHits; i++)
		{
			Entity* target = netsyncMapper->getEntity(p_identitiesHit[i]);
			if(target != NULL)
			{
				Transform* transform = static_cast<Transform*>(target->getComponent(
					ComponentType::Transform));
				if(transform != NULL)
				{
					geometricMean += transform->getTranslation();
				}
			}
		}
		geometricMean /= (float)p_numberOfHits;

		for(int i=0; i<p_numberOfHits; i++)
		{
			animateHit(entitySource, p_identitiesHit[i], geometricMean);
		}
	}
}

void TeslaEffectSystem::animateHit( Entity* p_fromEntity, int p_toEntity,
	const AglVector3 p_geometricMean )
{
	NetsyncDirectMapperSystem* netsyncMapper = static_cast<NetsyncDirectMapperSystem*>(
		m_world->getSystem(SystemType::NetsyncDirectMapperSystem));
	Entity* source = p_fromEntity;
	Entity* target = netsyncMapper->getEntity(p_toEntity);
	if(source != NULL && target != NULL)
	{
		Transform* sourceTransform = static_cast<Transform*>(source->getComponent(
			ComponentType::Transform));
		SpawnPointSet* spawnPoints = static_cast<SpawnPointSet*>(source->getComponent(
			ComponentType::SpawnPointSet));
		Transform* targetTransform = static_cast<Transform*>(target->getComponent(
			ComponentType::Transform));
		if(spawnPoints->m_spawnPoints.size() > 0)
		{
			AglMatrix spawnTransform = spawnPoints->m_spawnPoints[0].spTransform;
			MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(
				p_fromEntity->getComponent(ComponentType::MeshOffsetTransform));
			spawnTransform *= offset->offset.inverse();
			spawnTransform *= sourceTransform->getMatrix();
			AglVector3 sourcePosition = spawnTransform.GetTranslation();
			animate(sourcePosition, targetTransform->getTranslation(),
				p_geometricMean);
		}
	}
}

void TeslaEffectSystem::animate( const AglVector3& p_sourcePosition,
	const AglVector3& p_targetPosition, const AglVector3 p_geometricMean )
{
	EntityFactory* factory = static_cast<EntityFactory*>(m_world->getSystem(
		SystemType::EntityFactory));
	// up is in-game forward.
	AglVector3 forwardScale = AglVector3::up() *
		(p_sourcePosition - p_targetPosition).length();
	float thicknessFactor = RandomUtil::randomInterval(1.0f, 5.0f);
	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(),
		p_targetPosition - p_sourcePosition);
	Entity* effectCenter = factory->createTeslaEffectPieceClient(forwardScale,
		thicknessFactor, rotation, p_sourcePosition);
}

bool TeslaEffectSystem::entityInSystem( Entity* p_checkEntity ) const
{
	if(p_checkEntity == NULL)
		return false;

	for(unsigned int i=0; i<getActiveEntities().size(); i++)
	{
		if(getActiveEntities()[i]->getIndex() == p_checkEntity->getIndex())
		{
			return true;
		}
	}
	return false;
}