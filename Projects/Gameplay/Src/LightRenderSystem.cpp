#include "LightRenderSystem.h"

#include "GraphicsBackendSystem.h"
#include "LightsComponent.h"
#include "Transform.h"
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <BufferFactory.h>
#include <GraphicsWrapper.h>
#include <InstanceData.h>
#include <LightInstanceData.h>
#include <LightMesh.h>

LightRenderSystem::LightRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::LightRenderSystem, 2,
	ComponentType::LightsComponent, ComponentType::Transform )
{
	m_gfxBackend = p_gfxBackend;

	m_box = NULL;
}


LightRenderSystem::~LightRenderSystem()
{
	delete m_box;
}

void LightRenderSystem::initialize()
{
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

	BufferFactory* bufferFactory = new BufferFactory( gfxWrapper->getDevice(),
		gfxWrapper->getDeviceContext() );

	m_box = bufferFactory->createLightBoxMesh();
	delete bufferFactory;
}

void LightRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_directionalLightInstances.clear();
	m_pointLightInstances.clear();
	m_spotLightInstances.clear();

	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
	
	for( unsigned int i=0; i< p_entities.size(); i++ ) 
	{
		LightsComponent* lightsComp = static_cast<LightsComponent*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::LightsComponent ) );
		Transform* trans = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );
		
		vector<Light>* lights = lightsComp->getLightsPtr();
		for( unsigned int j=0; j<lights->size(); j++ )
		{
			AglMatrix finalMat;
			AglMatrix baseMat = trans->getMatrix();
			finalMat = lights->at(j).offsetMat * baseMat;

			AglQuaternion rot = finalMat.GetRotation();
			AglMatrix rotMat = AglMatrix::createRotationMatrix(rot);
			AglVector3 dir( 0.0f, 0.0f, 1.0f);
			dir.transform( rotMat );

			LightInstanceData inst = lights->at(j).instanceData;
			inst.lightDir[0] = dir.x;
			inst.lightDir[1] = dir.y;
			inst.lightDir[2] = dir.z;
			inst.setWorldTransform( finalMat );

			if( inst.type == LightTypes::E_LightTypes_DIRECTIONAL) {
				m_directionalLightInstances.push_back( inst );
			} else if( inst.type == LightTypes::E_LightTypes_POINT) {
				m_pointLightInstances.push_back( inst );
			} else if( inst.type == LightTypes::E_LightTypes_SPOT) {
				m_spotLightInstances.push_back( inst );
			}
		}
	}
}

void LightRenderSystem::render()
{
	// TODO: render different light types with different meshes

	if( m_directionalLightInstances.size() > 0 ) {
		m_gfxBackend->getGfxWrapper()->renderLights( m_box, &m_directionalLightInstances );
	}
	if( m_pointLightInstances.size() > 0 ) {
		m_gfxBackend->getGfxWrapper()->renderLights( m_box, &m_pointLightInstances );
	}
	if( m_spotLightInstances.size() > 0 ) {
		m_gfxBackend->getGfxWrapper()->renderLights( m_box, &m_spotLightInstances );
	}
}
