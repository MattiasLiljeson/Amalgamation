#include "GlowAnimationSystem.h"
#include "GlowAnimation.h"

GlowAnimationSystem::GlowAnimationSystem()
	: EntitySystem(SystemType::GlowAnimationSystem, 3, ComponentType::RenderInfo,
	ComponentType::GlowAnimation, ComponentType::Transform)
{
}

void GlowAnimationSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		GlowAnimation* glowAnimation = static_cast<GlowAnimation*>(p_entities[i]->
			getComponent(ComponentType::GlowAnimation));
		glowAnimation->lifetime -= m_world->getDelta();
		if(glowAnimation->lifetime <= 0)
		{
			glowAnimation->enabled = false;
			
			// NOTE: Remove below code line:
			//glowAnimation->lifetime = glowAnimation->getMaxLifetime();
		}
		float lifetimeFactor = glowAnimation->lifetime / glowAnimation->getMaxLifetime();
		float r, g, b, a;
		// RGB goes to (1, 1, 1).
		r = glowAnimation->getStartColor().x * lifetimeFactor;
		g = glowAnimation->getStartColor().y * lifetimeFactor;
		b = glowAnimation->getStartColor().z * lifetimeFactor;
		// Glow goes to (0).
		a = glowAnimation->getStartColor().w * lifetimeFactor;
		glowAnimation->color = AglVector4(-r, g, b, a); // "-r" means additive.
	}
}