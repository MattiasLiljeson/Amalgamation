#include "SpeedFovAdjustSystem.h"
#include "CameraInfo.h"
#include "Transform.h"

const float SpeedFovAdjustSystem::PI = 3.1415926536f;

ValBuffer::ValBuffer()
{
	m_currIdx = 0;
	for( int i=0; i<VAL_CNT; i++ ) {
		m_vals[i] = 0.0f;
	}
}

float ValBuffer::getAvg()
{
	float sum = 0.0f;
	for( int i=0; i<VAL_CNT; i++ ) {
		sum += m_vals[i];
	}
	return sum/VAL_CNT;
}

float ValBuffer::getLog()
{
	float mult = 0.5f;
	float sum = 0.0f;
	int idx = m_currIdx;
	int iterations = 0;
	while( iterations<VAL_CNT ){
		iterations++;
		sum += m_vals[idx]*mult;
		mult /= 2.0f;
		idx++;
		idx %= VAL_CNT;
	}
	return sum;
}

void ValBuffer::addVal( float p_val )
{
	m_currIdx++;
	m_currIdx %= VAL_CNT;
	m_vals[m_currIdx] = p_val;
}




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

void SpeedFovAdjustSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( !p_entities.empty() )
	{
		Entity* ship = m_world->getEntityManager()->
			getFirstEntityByComponentType( ComponentType::TAG_MyShip );

		if( ship != NULL )
		{

			Transform* shipTransform = static_cast<Transform*>(
				ship->getComponent( ComponentType::Transform ) );

			CameraInfo* cam = static_cast<CameraInfo*>(
				p_entities[0]->getComponent( ComponentType::CameraInfo ) );

			AglVector3 newPos = shipTransform->getTranslation();
			float speed = AglVector3::length( newPos - m_oldPos );
			m_oldPos = newPos;
			speed *= m_world->getDelta();
			m_buffer.addVal(speed);

			float fov = m_stdFov + m_buffer.getAvg()*50;
			float fovasRadians = (fov/360.0f) * 2.0f*PI;
			cam->m_fieldOfViewAsRadians = fovasRadians;
			cam->createPerspectiveMatrix();
		}
	}
}
