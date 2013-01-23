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
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
	
	gfxWrapper->beginLightPass();			  // finalize, draw to back buffer
	vector<LightInstanceData> instDatas;
	
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
			//AglMatrix scale = AglMatrix::createScaleMatrix( lights->at(j).scale );
			//AglMatrix rotation = AglMatrix::createRotationMatrix( lights->at(j).rotation );
			//AglMatrix translation = AglMatrix::createTranslationMatrix( lights->at(j).translation );
			//final =  scale * rotation * translation * base;
			final = lights->at(j).offset * base;

			LightInstanceData inst = lights->at(j).instanceData;
			inst.setWorldTransform( final );
			instDatas.push_back( inst );
		}
	}

	gfxWrapper->renderLights( m_box, &instDatas );
	 
	gfxWrapper->endLightPass();
}
