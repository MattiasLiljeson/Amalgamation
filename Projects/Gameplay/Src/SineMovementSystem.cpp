#include "SineMovementSystem.h"
#include "Transform.h"
#include "SineMovement.h"


SineMovementSystem::SineMovementSystem()
	: EntitySystem(SystemType::SineMovementSystem, 2, ComponentType::Transform,
	ComponentType::SineMovement)
{
}

void SineMovementSystem::inserted( Entity* p_entity )
{
	Transform* transform = static_cast<Transform*>(p_entity->getComponent(
		ComponentType::Transform));
	SineMovement* sine = static_cast<SineMovement*>(p_entity->getComponent(
		ComponentType::SineMovement));
	sine->originTranslation = transform->getTranslation();
}

void SineMovementSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		SineMovement* sine = static_cast<SineMovement*>(p_entities[i]->getComponent(
			ComponentType::SineMovement));
		if(sine->cycleTime > 0.0f)
		{
			sine->radian += m_world->getDelta() / sine->cycleTime;
			float pi = 3.14159265f;
			while(sine->radian >= pi * 2) {
				sine->radian -= (float)pi * 2;
			}
			AglVector3 translation = sine->originTranslation +
				sine->vector * sinf(sine->radian);
			transform->setTranslation(translation);
		}
	}
}