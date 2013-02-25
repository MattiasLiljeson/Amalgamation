#include "GravityMineEffectSystem.h"
#include "GravityMineEffectPiece.h"
#include <RandomUtil.h>
#include "EntityParent.h"

GravityMineEffectSystem::GravityMineEffectSystem()
	: EntitySystem(SystemType::GravityMineEffectSystem, 4,
	ComponentType::GravityMineEffectPiece, ComponentType::Transform,
	ComponentType::EntityParent, ComponentType::RenderInfo)
{
}

void GravityMineEffectSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		GravityMineEffectPiece* piece = static_cast<GravityMineEffectPiece*>(
			p_entities[i]->getComponent(ComponentType::GravityMineEffectPiece));
		piece->lifeTime -= m_world->getDelta() * 1.0f;
		if(piece->lifeTime <= 0.0f)
		{
			while(piece->lifeTime <= 0.0f)
			{
				piece->lifeTime += piece->maxLifeTime;
			}
			RandomUtil::randomEvenlyDistributedSphere(&piece->offsetVector.x,
				&piece->offsetVector.y, &piece->offsetVector.z);
			piece->offsetVector *= piece->radius;
			AglMatrix localTransform;
			AglMatrix::componentsToMatrix(localTransform, AglVector3(1.0f, 1.0f, 1.0f),
				AglQuaternion::rotateToFrom(AglVector3::forward(), piece->offsetVector),
				piece->offsetVector);
			EntityParent* parent = static_cast<EntityParent*>(p_entities[i]->
				getComponent(ComponentType::EntityParent));
			parent->setLocalTransform(localTransform);
		}

		float lifeTimeFraction = piece->lifeTime / piece->maxLifeTime;
		AglMatrix localTransform;
		AglVector3 scale = AglVector3::one() - AglVector3(lifeTimeFraction, lifeTimeFraction, lifeTimeFraction);
		//AglVector3 scale = AglVector3(lifeTimeFraction, lifeTimeFraction, lifeTimeFraction);
		AglMatrix::componentsToMatrix(localTransform, scale,
			AglQuaternion::rotateToFrom(AglVector3::up(), -piece->offsetVector),
			piece->offsetVector * sqrt(lifeTimeFraction));
		EntityParent* parent = static_cast<EntityParent*>(p_entities[i]->
			getComponent(ComponentType::EntityParent));
		parent->setLocalTransform(localTransform);
	}
}