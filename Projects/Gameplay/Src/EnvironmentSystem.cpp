#include "EnvironmentSystem.h"

#include "EnvironmentValues.h"
#include "Transform.h"
#include "CameraInfo.h"
#include <ValueClamp.h>

EnvironmentSystem::EnvironmentSystem() : 
	EntitySystem( SystemType::EnvironmentSystem, 2,
				  ComponentType::ComponentTypeIdx::EnvironmentValues,
				  ComponentType::ComponentTypeIdx::Transform)
{

}

EnvironmentSystem::~EnvironmentSystem()
{

}

void EnvironmentSystem::initialize()
{

}

void EnvironmentSystem::processEntities( const vector<Entity*>& p_entities )
{
	// get camera
	if (p_entities.size()>0)
	{	
		// Get the current camera
		Transform* camTransform=NULL;
		CameraInfo* camInfo=NULL;
		auto entitymanager = m_world->getEntityManager();
		Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
		if (cam)
		{
			camTransform = static_cast<Transform*>(
				cam->getComponent( ComponentType::Transform ) );
			camInfo = static_cast<CameraInfo*>(
				cam->getComponent( ComponentType::CameraInfo ) );
		}
		// sum
		AglVector3 sumFogColor;
		AglVector3 sumAmbientColor;
		float sumFogNear=1.0f, sumFogFar=1.0f;
		float sumAtt=0.0f;
		//
		if (camTransform && camInfo)
		{
			for( unsigned int i=0; i< p_entities.size(); i++ ) 
			{
				Entity* entity = p_entities[i];
				Transform* transform = static_cast<Transform*>(
					entity->getComponent( ComponentType::Transform ) );

				EnvironmentValues* environmentValues = static_cast<EnvironmentValues*>(
					entity->getComponent( ComponentType::EnvironmentValues ) );

				AglVector3 thisFogColor=environmentValues->m_fogColor;
				AglVector3 thisAmbientColor=environmentValues->m_ambientColor;
				float thisFogNear=environmentValues->m_fogNearPlaneClosenessPercentage, 
					thisFogFar=environmentValues->m_fogFarPlaneClosenessPercentage;

				float att = 1.0f;

				if (transform && environmentValues)
				{
					float dist = AglVector3::length(
						camTransform->getTranslation()-transform->getTranslation());
					// Calculate linear attenuation
					float min = environmentValues->m_minRadius;
					float max = environmentValues->m_maxRadius;
					att = 1.0f-((clamp(dist, min, max+min)-min)/max);
				}

				sumFogColor+=thisFogColor*att;
				sumAmbientColor+=thisAmbientColor*att;
				sumFogNear+=thisFogNear*att; 
				sumFogFar+=thisFogFar*att;
				sumAtt+=att;

			}

		}
		// calculate total averages rumble
		if (sumAtt>0.00001f)
		{
			float divisor = max(1.0f,sumAtt);
			sumFogColor/=divisor;
			sumAmbientColor/=divisor;
			sumFogNear/=divisor; 
			sumFogFar/=divisor;
		}
		sumFogFar=saturate(sumFogFar);
		sumFogNear=saturate(sumFogNear);
		// set the resulting values
		camInfo->m_fogColor=sumFogColor;
		camInfo->m_ambientColor=sumAmbientColor;
		camInfo->m_fogNearPlaneClosenessPercentage = sumFogNear;
		camInfo->m_fogFarPlaneClosenessPercentage = sumFogFar;

	}

}
