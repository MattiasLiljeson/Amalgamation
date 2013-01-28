#include "LightBlinkerSystem.h"
#include "LightsComponent.h"
#include "LightBlinker.h"


LightBlinkerSystem::LightBlinkerSystem()
	: EntitySystem( SystemType::LightBlinkerSystem, 2, ComponentType::LightBlinker,
	ComponentType::LightsComponent)
{
}

LightBlinkerSystem::~LightBlinkerSystem()
{
}

void LightBlinkerSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		LightsComponent* lightComponent = static_cast<LightsComponent*>(p_entities[i]->getComponent(
			ComponentType::LightsComponent));
		LightBlinker* lightBlinker = static_cast<LightBlinker*>(p_entities[i]->getComponent(
			ComponentType::LightBlinker));
		for(unsigned int lightIndex=0; lightIndex<lightComponent->getLightsPtr()->size();
			lightIndex++)
		{
			float range = (*lightComponent->getLightsPtr())[lightIndex].instanceData.range;
			if(lightBlinker->increase)
			{
				range += m_world->getDelta() * 20.0f;
				if(range >= lightBlinker->maxRange)
				{
					lightBlinker->increase = false;
				}
			}
			else
			{
				range -= m_world->getDelta() * 20.0f;
				if(range <= 1.0f)
				{
					lightBlinker->increase = true;
				}
			}
			(*lightComponent->getLightsPtr())[lightIndex].instanceData.range = range;
			(*lightComponent->getLightsPtr())[lightIndex].instanceData.attenuation[0] =
				lightBlinker->maxRange / range;
			AglMatrix::componentsToMatrix(
				(*lightComponent->getLightsPtr())[lightIndex].offsetMat,
				AglVector3( range, range, range ),
				AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
				AglVector3( 0.0f, 0.0f, 0.0f )
			);
		}
	}
}

void LightBlinkerSystem::inserted( Entity* p_entity )
{

}
