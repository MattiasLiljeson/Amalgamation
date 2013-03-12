#include "TeslaEffectSystem.h"
#include "Transform.h"
#include "LoadMesh.h"
#include "TeslaEffectPiece.h"
#include "NetsyncDirectMapperSystem.h"
#include <RandomUtil.h>
#include "EntityFactory.h"
#include "SpawnPointSet.h"
#include "MeshOffsetTransform.h"
#include "TeslaCoilEffect.h"

TeslaEffectSystem::TeslaEffectSystem()
	: EntitySystem(SystemType::TeslaEffectSystem, 6, ComponentType::Transform,
	ComponentType::MeshOffsetTransform, ComponentType::SpawnPointSet,
	ComponentType::TeslaCoilModule, ComponentType::ShipModule,
	ComponentType::TeslaCoilEffect)
{
}

void TeslaEffectSystem::inserted( Entity* p_entity )
{
	TeslaCoilEffect* coilEffect = static_cast<TeslaCoilEffect*>(p_entity->getComponent(
		ComponentType::TeslaCoilEffect));
	EntityFactory* factory = static_cast<EntityFactory*>(m_world->getSystem(
		SystemType::EntityFactory));
	for(unsigned int meshIndex=0; meshIndex<coilEffect->possibleMeshes.size(); meshIndex++)
	{
		Entity* entity = NULL;
		vector<Entity*> entityPool;
		for(unsigned int i=0; i<10; i++)
		{
			entity = factory->createTeslaEffectPieceClient(AglVector3::one(), 1.0f,
				AglQuaternion::identity(), AglVector3::zero(),
				coilEffect->possibleMeshes[meshIndex]);
			entityPool.push_back(entity);
		}
		m_lightningPool[p_entity->getIndex()].push_back( entityPool );
	}
}

void TeslaEffectSystem::animateHits( int p_fromEntity, int* p_identitiesHit,
	int p_numberOfHits, int* p_identitiesHitFloating, int p_numberOfHitsFloating )
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
			animateHit(entitySource, p_identitiesHit[i], geometricMean, i, true);
		}
		for(int i=p_numberOfHits; i<p_numberOfHitsFloating; i++)
		{
			animateHit(entitySource, p_identitiesHitFloating[i], geometricMean, i, false);
		}
	}
}

void TeslaEffectSystem::animateHit( Entity* p_fromEntity, int p_toEntity,
	const AglVector3 p_geometricMean, int p_index, bool p_damage )
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
			int randomMesh = RandomUtil::randomInteger(
				(int)m_lightningPool[p_fromEntity->getIndex()].size() - 1);
			Entity* effectEntity = m_lightningPool[p_fromEntity->getIndex()][randomMesh][p_index];
			
			Transform* ligntningTransform = static_cast<Transform*>(effectEntity->
				getComponent(ComponentType::Transform));
			ligntningTransform->setTranslation(sourcePosition);
			AglVector3 forwardScale = AglVector3::up() *
				(sourcePosition - targetTransform->getTranslation()).length();
			// right is in-game up.
			AglVector3 upScale = AglVector3::right();
			// forward is in-game right.
			AglVector3 rightScale = AglVector3::forward();
			if(!p_damage)
			{
				upScale /= 5.0f;
				rightScale /= 5.0f;
			}
			AglVector3 scale = forwardScale + upScale + rightScale;
			ligntningTransform->setScale(scale);
			AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(),
				targetTransform->getTranslation() - sourcePosition);
			ligntningTransform->setRotation(rotation);

			TeslaEffectPiece* effectPiece = static_cast<TeslaEffectPiece*>(
				effectEntity->getComponent(ComponentType::TeslaEffectPiece));
			effectPiece->lifeTime = effectPiece->maxLifeTime; // Restarts it.
			effectPiece->forwardScale = forwardScale;
			effectPiece->damages = p_damage;
			
			//animate(sourcePosition, targetTransform->getTranslation(),
			//	p_geometricMean, );
		}
	}
}

//void TeslaEffectSystem::animate( const AglVector3& p_sourcePosition,
//	const AglVector3& p_targetPosition, const AglVector3 p_geometricMean,
//	string p_meshName)
//{
//	EntityFactory* factory = static_cast<EntityFactory*>(m_world->getSystem(
//		SystemType::EntityFactory));
//	// up is in-game forward.
//	AglVector3 forwardScale = AglVector3::up() *
//		(p_sourcePosition - p_targetPosition).length();
//	float thicknessFactor = RandomUtil::randomInterval(1.0f, 5.0f);
//	AglQuaternion rotation = AglQuaternion::rotateToFrom(AglVector3::up(),
//		p_targetPosition - p_sourcePosition);
//	Entity* effectCenter = factory->createTeslaEffectPieceClient(forwardScale,
//		thicknessFactor, rotation, p_sourcePosition, p_meshName);
//}

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