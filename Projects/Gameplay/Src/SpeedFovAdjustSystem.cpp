#include "SpeedFovAdjustSystem.h"
#include "CameraInfo.h"
#include "Transform.h"
#include <AntTweakBarWrapper.h>
#include "SpeedBuffer.h"
#include "ThrustComponent.h"

const float SpeedFovAdjustSystem::PI = 3.1415926536f;

SpeedFovAdjustSystem::SpeedFovAdjustSystem( float p_fov ) : 
	EntitySystem( SystemType::InterpolationSystem, 3,
	ComponentType::CameraInfo,
	ComponentType::TAG_MainCamera,
	ComponentType::Transform )
{
	m_stdFov = p_fov;
}

SpeedFovAdjustSystem::~SpeedFovAdjustSystem()
{
}

void SpeedFovAdjustSystem::initialize()
{
	m_speedFovMult = 0.0;

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::GRAPHICS,
		"Fov speed mult", TwType::TW_TYPE_FLOAT, &m_speedFovMult, "" );
}

void SpeedFovAdjustSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( !p_entities.empty() )
	{
		Entity* ship = m_world->getEntityManager()->
			getFirstEntityByComponentType( ComponentType::TAG_MyShip );

		if( ship != NULL )
		{

			CameraInfo* cam = static_cast<CameraInfo*>(
				p_entities[0]->getComponent( ComponentType::CameraInfo ) );

			ThrustComponent* thrust = static_cast<ThrustComponent*>
				(ship->getComponent(ComponentType::ThrustComponent));

			float factor = thrust->m_thrustPower / (float)thrust->POWERCAP + 1.0f;

			cam->m_fieldOfViewAsRadians = 0.785398163f * factor;
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
