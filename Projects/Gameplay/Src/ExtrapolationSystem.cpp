#include "ExtrapolationSystem.h"
#include <TcpClient.h>
#include <AntTweakBarWrapper.h>

// Components
#include "Extrapolate.h"
#include "Transform.h"

ExtrapolationSystem::ExtrapolationSystem( TcpClient* p_client )
	: EntitySystem( SystemType::ExtrapolationSystem, 2,
	ComponentType::Extrapolate, ComponentType::Transform )
{
	m_client = p_client;
	m_correctedDeltaTime = 0;
}

ExtrapolationSystem::~ExtrapolationSystem()
{
}

void ExtrapolationSystem::processEntities( const vector<Entity*>& p_entities )
{
	float latestUpdateTimeStamp = searchForLatestUpdateTimeStamp( p_entities );
	unsigned int numEntities = p_entities.size();

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		Extrapolate* extrapolate = NULL;
		extrapolate = static_cast<Extrapolate*>(p_entities[i]->getComponent(
			ComponentType::Extrapolate));

		Transform* transform = NULL;
		transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));

		AglVector3 translation = transform->getTranslation();
		AglVector3 velocity = extrapolate->velocityVector;
		AglVector3 angularVelocity = extrapolate->angularVelocity;

//		float packetUpdateTime = extrapolate->serverUpdateTimeStamp;
//		m_correctedDeltaTime = latestUpdateTimeStamp - packetUpdateTime;

		m_correctedDeltaTime = m_world->getDelta() + latestUpdateTimeStamp -
			extrapolate->serverUpdateTimeStamp;

		// Extrapolate translation
		translation += (velocity * m_correctedDeltaTime);
		transform->setTranslation( translation );

//		// Extrapolate orientation (Must be fixed because it rotates modules locally.)
//		AglQuaternion orientation = transform->getRotation();
//		AglQuaternion velocityQuaternion = AglQuaternion::constructFromAngularVelocity(
//			angularVelocity * m_correctedDeltaTime );
//		orientation = velocityQuaternion * orientation;
//		transform->setRotation( orientation );

	}
}

void ExtrapolationSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::NETWORK, "Corrected delta",
		TwType::TW_TYPE_FLOAT, &m_correctedDeltaTime, "" );
}

float ExtrapolationSystem::searchForLatestUpdateTimeStamp( const vector<Entity*>& p_entities )
{
	float latestUpdateTimeStamp = 0;
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		Extrapolate* extrapolate = NULL;
		extrapolate = static_cast<Extrapolate*>(p_entities[i]->getComponent(
			ComponentType::Extrapolate));

		if( extrapolate->serverUpdateTimeStamp > latestUpdateTimeStamp )
		{
			latestUpdateTimeStamp = extrapolate->serverUpdateTimeStamp;
		}
	}

	return latestUpdateTimeStamp;
}