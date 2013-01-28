#include "MoveShipLightsSystem.h"
#include <AntTweakBarWrapper.h>
#include "LightsComponent.h"
#include <AglMatrix.h>


MoveShipLightsSystem::MoveShipLightsSystem()
	: EntitySystem(SystemType::MoveShipLightsSystem, 2, ComponentType::ShipFlyController,
	ComponentType::LightsComponent)
{
	m_lightPosition[0] = 3.0f;
	m_lightPosition[1] = 0;
	m_lightPosition[2] = -2.0f;
}

MoveShipLightsSystem::~MoveShipLightsSystem()
{
}

void MoveShipLightsSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addWriteVariable(AntTweakBarWrapper::OVERALL,
		"light", TwType::TW_TYPE_DIR3F, &m_lightPosition, "");
}

void MoveShipLightsSystem::processEntities( const vector<Entity*>& p_entities )
{
	//if(p_entities.size() == 1)
	//{
	//	LightsComponent* lights = static_cast<LightsComponent*>(p_entities[0]->getComponent(
	//		ComponentType::LightsComponent));
	//	float range = 2.0f;
	//	// Left light
	//	AglMatrix::componentsToMatrix(
	//		(*lights->getLightsPtr())[2].offsetMat,
	//		AglVector3( range, range, range ),
	//		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
	//		AglVector3( -m_lightPosition[0], m_lightPosition[1], m_lightPosition[2] )
	//		);
	//	//// Right light
	//	//AglMatrix::componentsToMatrix(
	//	//	(*lights->getLightsPtr())[3].offsetMat,
	//	//	AglVector3( range, range, range ),
	//	//	AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
	//	//	AglVector3( m_lightPosition[0], m_lightPosition[1], m_lightPosition[2] )
	//	//	);
	//}
}