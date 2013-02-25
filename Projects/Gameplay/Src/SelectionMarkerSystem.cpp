#include "SelectionMarkerSystem.h"
#include "SelectionMarker.h"
#include "RenderInfo.h"
#include "Transform.h"
#include "SkeletalAnimation.h"


SelectionMarkerSystem::SelectionMarkerSystem() : 
	EntitySystem( SystemType::SelectionMarkerSystem, 1,
	ComponentType::SelectionMarker)

{
}

SelectionMarkerSystem::~SelectionMarkerSystem()
{

}

void SelectionMarkerSystem::initialize()
{
	m_marker = m_world->createEntity();

	m_marker->addComponent(ComponentType::SelectionMarker, new SelectionMarker());
	m_marker->addComponent(ComponentType::RenderInfo, new RenderInfo(false));
	m_marker->addComponent(ComponentType::Transform, new Transform());

	m_world->addEntity(m_marker);
}

void SelectionMarkerSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		SelectionMarker* marker = static_cast<SelectionMarker*>(p_entities[i]->getComponent(ComponentType::SelectionMarker));
		RenderInfo* markerRenderInfo = static_cast<RenderInfo*>(p_entities[i]->getComponent(ComponentType::RenderInfo));
		if (marker->m_targetEntity >= 0)
		{
			Entity* target = m_world->getEntity(marker->m_targetEntity);
			RenderInfo* targetRenderInfo = static_cast<RenderInfo*>(target->getComponent(ComponentType::RenderInfo));
			markerRenderInfo->m_meshId = targetRenderInfo->m_meshId;
			markerRenderInfo->m_shouldBeRendered = true;
		}
		else
		{
			markerRenderInfo->m_shouldBeRendered = false;
		}
	}
}

void SelectionMarkerSystem::setMarkerTarget(int p_target, AglMatrix p_transform)
{
	SelectionMarker* marker = static_cast<SelectionMarker*>(m_marker->getComponent(ComponentType::SelectionMarker));
	Transform* trans = static_cast<Transform*>(m_marker->getComponent(ComponentType::Transform));


	trans->setMatrix(p_transform);
	marker->m_targetEntity = p_target;
	if (p_target >= 0)
	{
		Entity* t = m_world->getEntity(p_target);
		SkeletalAnimation* skelAnimTarget = static_cast<SkeletalAnimation*>(t->getComponent(ComponentType::SkeletalAnimation));
		if (skelAnimTarget)
		{
			SkeletalAnimation* skelAnim = static_cast<SkeletalAnimation*>(m_marker->getComponent(ComponentType::SkeletalAnimation));
			if (!skelAnim)
			{
				skelAnim = new SkeletalAnimation(0, skelAnimTarget->m_scene, skelAnimTarget->m_offset);
				skelAnim->m_playSpeed = 0;
				m_marker->addComponent(ComponentType::SkeletalAnimation, skelAnim);
			}
			else
			{
				skelAnim->m_offset = skelAnimTarget->m_offset;
				skelAnim->m_scene = skelAnimTarget->m_scene;
			}
		}
		else
		{
			m_marker->removeComponent(ComponentType::SkeletalAnimation);
			m_marker->applyComponentChanges();
		}
	}
}