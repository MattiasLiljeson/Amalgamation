#include "ClientMeasurementSystem.h"
#include <AntTweakBarWrapper.h>
#include "MeshRenderSystem.h"
#include "CullingSystem.h"
#include "ParticleRenderSystem.h"
#include "PortalCullingSystem.h"

ClientMeasurementSystem::ClientMeasurementSystem()
	: EntitySystem( SystemType::ClientMeasurementSystem )
{
	m_ESTime = 0.0;
	m_deltaTime = 0.0;
}

ClientMeasurementSystem::~ClientMeasurementSystem()
{
}

void ClientMeasurementSystem::process()
{
	m_ESTime = 1000.0 * ((double)m_world->getDelta() - m_world->getTotalSystemsTime());
}

void ClientMeasurementSystem::initialize()
{
	initMeasuredSystems();

	AntTweakBarWrapper::getInstance()->modifyTheRefreshRate(
		AntTweakBarWrapper::MEASUREMENT, 0.1f );
	AntTweakBarWrapper::getInstance()->defineParametersForABar(
		AntTweakBarStrings::measurement.c_str(), " valueswidth=120");

	vector<EntitySystem*> systems = m_world->getSystems()->getSystemList();
	for(unsigned int i=0; i<m_measuredSystems.size(); i++)
	{
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT, m_measuredSystems[i].second.c_str(),
			TwType::TW_TYPE_DOUBLE, &m_measuredSystems[i].first->getTimeUsedPerSecond(),
			"group='Per second ratio' precision='6'" );
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT, (m_measuredSystems[i].second+" ").c_str(),
			TwType::TW_TYPE_DOUBLE, &m_measuredSystems[i].first->getAverageExecutionTime(),
			"group='Average process execution time (ms)' precision='4'" );
	}

	//Culling
	CullingSystem* cull = static_cast<CullingSystem*>(m_world->getSystem(SystemType::CullingSystem));
	PortalCullingSystem* cull2 = static_cast<PortalCullingSystem*>(m_world->getSystem(SystemType::PortalCullingSystem));

	ParticleRenderSystem* psSystem = static_cast<ParticleRenderSystem*>(m_world->getSystem(SystemType::ParticleRenderSystem));

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Rendered",
		TwType::TW_TYPE_UINT32, cull->getRenderedCountPtr(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Culled",
		TwType::TW_TYPE_UINT32, cull->getCulledCountPtr(),
		"group='Culling'" );
	 
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Culled Fraction",
		TwType::TW_TYPE_FLOAT, cull->getCulledFractionPtr(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Tesselated Meshes",
		TwType::TW_TYPE_UINT32, cull->getTesselatedCountPtr(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Rendered Particle Systems",
		TwType::TW_TYPE_INT32, psSystem->getDrawnPSCount(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Max Rendered Particle Systems",
		TwType::TW_TYPE_INT32, psSystem->getMaxDrawnPSCount(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Rendered Chambers",
		TwType::TW_TYPE_INT32, cull2->getRenderedChambers(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Culled Chambers",
		TwType::TW_TYPE_INT32, cull2->getCulledChambers(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Collides with gate",
		TwType::TW_TYPE_BOOLCPP, cull2->getCollidesWithGate(),
		"group='Culling'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "Collides with chamber",
		TwType::TW_TYPE_BOOLCPP, cull2->getCollidesWithChamber(),
		"group='Culling'" );

	//ES time
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT, "ES Time",
		TwType::TW_TYPE_DOUBLE, &m_ESTime, "group='other'" );
}

void ClientMeasurementSystem::initMeasuredSystems()
{
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::ClientPacketHandlerSystem),
		"ClientPacketHandler"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::GraphicsRendererSystem),
		"GraphicsRenderer"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::RenderPrepSystem),
		"InstanceBaking"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::ShieldPlatingSystem),
		"ShieldPlating"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::ShieldPlaterSystem),
		"ShieldPLATER"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::AnomalyBombEffectSystem),
		"GravityField"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::TransformParentHandlerSystem),
		"TransformHierarchy"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::AxisRotationSystem),
		"AxisRotation"));

	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::CullingSystem),
		"Cull"));
}