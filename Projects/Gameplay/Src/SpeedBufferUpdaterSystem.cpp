#include "SpeedBufferUpdaterSystem.h"
#include "SpeedBuffer.h"
#include "Transform.h"


SpeedBufferUpdaterSystem::SpeedBufferUpdaterSystem()
	: EntitySystem (SystemType::SpeedBufferUpdaterSystem, 2,
	ComponentType::SpeedBuffer,
	ComponentType::Transform)
{
}


SpeedBufferUpdaterSystem::~SpeedBufferUpdaterSystem()
{
}

void SpeedBufferUpdaterSystem::initialize()
{

}

void SpeedBufferUpdaterSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int entIdx = 0; entIdx<p_entities.size(); entIdx++ )
	{
		Transform* transform = static_cast<Transform*>(
			p_entities[entIdx]->getComponent( ComponentType::Transform ) );

		SpeedBuffer* speedBuffer = static_cast<SpeedBuffer*>(
			p_entities[entIdx]->getComponent( ComponentType::SpeedBuffer ) );

		AglVector3 newPos = transform->getTranslation();
		float speed = AglVector3::length( newPos - speedBuffer->m_oldPos );
		speedBuffer->m_oldPos = newPos;
		speed /= m_world->getDelta();
		speedBuffer->m_buffer.addVal( speed );

	}
}
