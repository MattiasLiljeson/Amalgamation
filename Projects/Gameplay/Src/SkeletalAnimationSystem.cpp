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
		if (anim->m_currentTake >= 0)
		{
			AnimationTake take = anim->m_takes[anim->m_currentTake];

			if (take.endFrame == take.startFrame)
			{
				anim->m_time = take.startFrame / anim->m_fps;

				if (anim->queuedTakes.size() > 0)
				{
					anim->m_currentTake = anim->queuedTakes.front();
					anim->queuedTakes.pop_front();
					if (anim->m_takes[anim->m_currentTake].backwards)
						anim->m_time = anim->m_takes[anim->m_currentTake].endFrame / anim->m_fps;
					else
						anim->m_time = anim->m_takes[anim->m_currentTake].startFrame / anim->m_fps;
				}
			}
			else
			{
				if (take.backwards)
					anim->m_time -= dt * take.speed;
				else
					anim->m_time += dt * take.speed;

				float maxTime = take.endFrame / anim->m_fps;//anim->m_scene->getAnimation(0)->getMaxTime();
				float minTime = take.startFrame / anim->m_fps;//anim->m_scene->getAnimation(0)->getMinTime();

				if (anim->m_time > maxTime || anim->m_time < minTime)
				{
					if (anim->queuedTakes.size() > 0)
					{
						anim->m_currentTake = anim->queuedTakes.front();
						anim->queuedTakes.pop_front();
						if (anim->m_takes[anim->m_currentTake].backwards)
							anim->m_time = anim->m_takes[anim->m_currentTake].endFrame / anim->m_fps;
						else
							anim->m_time = anim->m_takes[anim->m_currentTake].startFrame / anim->m_fps;
					}
					else
					{
						while (anim->m_time > maxTime)
							anim->m_time -= maxTime-minTime;
						while (anim->m_time < minTime)
							anim->m_time += maxTime-minTime;
					}
				}
			}
		}
		else
		{
			anim->m_time = 0;
		}
	}
}