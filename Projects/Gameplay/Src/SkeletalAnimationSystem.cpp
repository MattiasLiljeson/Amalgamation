#include "SkeletalAnimationSystem.h"
#include "SkeletalAnimation.h"

SkeletalAnimationSystem::SkeletalAnimationSystem() : EntitySystem( SystemType::SkeletalAnimationSystem, 1, 
											ComponentType::SkeletalAnimation){

}

SkeletalAnimationSystem::~SkeletalAnimationSystem()
{

}

void SkeletalAnimationSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		SkeletalAnimation* anim = static_cast<SkeletalAnimation*>(p_entities[i]->getComponent(ComponentType::SkeletalAnimation));
		anim->m_time += dt;
	}
}