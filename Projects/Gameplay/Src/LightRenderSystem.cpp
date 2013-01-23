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
	m_data.clear();

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
			AglMatrix final;
			AglMatrix base = trans->getMatrix();
			final = lights->at(j).offset * base;

			LightInstanceData inst = lights->at(j).instanceData;
			inst.setWorldTransform( final );
			m_data.push_back( inst );
		}
	}
}

void LightRenderSystem::render()
{
	m_gfxBackend->getGfxWrapper()->renderLights( m_box, &m_data );
}
