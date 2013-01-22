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
			AglMatrix scale = AglMatrix::createScaleMatrix( lights->at(j).scale );
			AglMatrix rotation = AglMatrix::createRotationMatrix( lights->at(j).rotation );
			AglMatrix translation = AglMatrix::createTranslationMatrix( lights->at(j).translation );
			final =  scale * base * rotation * translation;

			LightInstanceData inst = lights->at(j).instanceData;
			inst.setWorldTransform( final );
			instDatas.push_back( inst );
		}
	}

	// hard coded lights
	LightInstanceData instData;
	float range = 10.0f;
	instData.range = range;
	instData.worldTransform[0] = range;
	instData.worldTransform[5] = range;
	instData.worldTransform[10] = range;
	//instData.setScale( AglVector3( 10.0, 10.0, 10.0 ) );
	instData.attenuation[0] = 1.1f;
	instData.attenuation[1] = 0.01f;
	instData.attenuation[2] = 0.1f;
	instData.spotPower = 100.0f;
	instData.specular[3] = 1.0f;
	instData.type = LightTypes::E_LightTypes_POINT;

	instData.ambient[2] = 0.005f;

	float intensitity = 1.0f;
	for( int x=0; x<5; x++ )
	{
		instData.worldTransform[3] = x * (range+1.0f) - 25.0f;
		instData.diffuse[0] += intensitity;
		for( int y=0; y<5; y++ )
		{
			instData.worldTransform[7] = y * (range+1.0f) - 25.0f;
			instData.diffuse[1] += intensitity / 5.0f;
			for( int z=0; z<5; z++ )
			{
				instData.worldTransform[11] = z * (range+1.0f) - 25.0f;
				instData.diffuse[2] += intensitity / 25.0f;
				//instDatas.push_back( instData );
			}
		}
	}

	gfxWrapper->renderLights( m_box, &instDatas );
	 
	gfxWrapper->endLightPass();
}
