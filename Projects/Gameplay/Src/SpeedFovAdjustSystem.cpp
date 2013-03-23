#include "SpeedFovAdjustSystem.h"
#include "CameraInfo.h"
#include "Transform.h"
#include <AntTweakBarWrapper.h>
#include "SpeedBuffer.h"
#include "ThrustComponent.h"


SpeedFovAdjustSystem::SpeedFovAdjustSystem( ) : 
	EntitySystem( SystemType::InterpolationSystem, 3,
	ComponentType::CameraInfo,
	ComponentType::TAG_MainCamera,
	ComponentType::Transform )
{
}

SpeedFovAdjustSystem::~SpeedFovAdjustSystem()
{
}

void SpeedFovAdjustSystem::initialize()
{
	m_fieldOfViewFactor = 2.0f;
	m_offest = 1.18f;
	m_maxAdjustment = 1.77f;

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::OVERALL,
		"Factor", TwType::TW_TYPE_FLOAT, &m_fieldOfViewFactor, "group=FOV step=0.01f" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::OVERALL,
		"Offset", TwType::TW_TYPE_FLOAT, &m_offest, "group=FOV step=0.01f" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::OVERALL,
		"MaxAdjustment", TwType::TW_TYPE_FLOAT, &m_maxAdjustment, "group=FOV step=0.01f" );

}

void SpeedFovAdjustSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( !p_entities.empty() )
	{
		Entity* ship = m_world->getEntityManager()->
			getFirstEntityByComponentType( ComponentType::TAG_MyShip );

		if( ship != NULL ){
			CameraInfo* cam = static_cast<CameraInfo*>(
				p_entities[0]->getComponent( ComponentType::CameraInfo ) );

			ThrustComponent* thrust = static_cast<ThrustComponent*>
				(ship->getComponent(ComponentType::ThrustComponent));

			float thrustFactor = thrust->m_thrustPower / (float)thrust->POWERCAP;

			thrustFactor = pow(thrustFactor, m_fieldOfViewFactor);

			float resultingFactor = thrustFactor + m_offest;

			if(resultingFactor >= m_maxAdjustment){
				resultingFactor = m_maxAdjustment;
			}

			cam->m_fieldOfViewAsRadians = 0.785398163f * resultingFactor;
			cam->createPerspectiveMatrix();

			/*
			SpeedBuffer* shipSpeedBuffer = static_cast<SpeedBuffer*>(
				ship->getComponent( ComponentType::SpeedBuffer ) );

			CameraInfo* cam = static_cast<CameraInfo*>(
				p_entities[0]->getComponent( ComponentType::CameraInfo ) );

			if( shipSpeedBuffer != NULL && cam != NULL) {
				float fov = m_stdFov + shipSpeedBuffer->m_buffer.getAvg() * m_speedFovMult;
				float fovasRadians = (fov/360.0f) * 2.0f*PI;
				cam->m_fieldOfViewAsRadians = fovasRadians;
				cam->createPerspectiveMatrix();
			}
			*/
		}
	}
}
