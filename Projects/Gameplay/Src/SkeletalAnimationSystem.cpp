#include "SkeletalAnimationSystem.h"
#include "SkeletalAnimation.h"
#include <AglScene.h>

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
		if (anim->m_isPlaying)
			anim->m_time += dt*anim->m_playSpeed;

		float maxTime = anim->m_scene->getAnimation(0)->getMaxTime();
		float minTime = anim->m_scene->getAnimation(0)->getMinTime();
		while (anim->m_time > maxTime)
		{
			anim->m_time -= maxTime-minTime;
		}
		if (anim->m_time < minTime)
			anim->m_time = minTime;
	}
}