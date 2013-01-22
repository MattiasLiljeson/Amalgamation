#include "LightRenderSystem.h"

#include "GraphicsBackendSystem.h"
#include <BufferFactory.h>
#include <GraphicsWrapper.h>
#include <InstanceData.h>
#include <LightInstanceData.h>
#include <LightMesh.h>

LightRenderSystem::LightRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::LightRenderSystem )
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
	static float range = 10.0f;

	AglMatrix mat = AglMatrix::identityMatrix();
	mat[0] = mat[5] = mat[10] =  range / 2.0f; // The cube is 2.0f wide, therefore 2 and not 1
	
	LightInstanceData instData;
	instData.range = range; // Should be synced with wolrdTransform
	for( int i=0; i<16; i++ ){
		instData.worldTransform[i] = mat[i];
	}

	instData.lightDir[0] = 1.0f;
	instData.lightDir[1] = 0.0f;
	instData.lightDir[2] = 0.0f;

	instData.attenuation[0] = 1.1f;
	instData.attenuation[1] = 0.01f;
	instData.attenuation[2] = 0.1f;
	instData.spotPower = 100.0f;

	instData.ambient[0] = 0.01f;
	instData.ambient[1] = 0.01f;
	instData.ambient[2] = 0.01f;
	instData.ambient[3] = 1.01f;

	instData.diffuse[0] = 0.0f;
	instData.diffuse[1] = 0.5f;
	instData.diffuse[2] = 0.0f;
	instData.diffuse[3] = 1.0f;

	instData.specular[0] = 0.5f;
	instData.specular[1] = 0.1f;
	instData.specular[2] = 0.0f;
	instData.specular[3] = 1.0f;

	instData.enabled = true;
	instData.type = LightTypes::E_LightTypes_POINT;

	for( int x=0; x<5; x++ )
	{
		instData.worldTransform[3] = x * (range+1.0f) - 25.0f;
		instData.diffuse[1] += 0.1f;
		for( int y=0; y<5; y++ )
		{
			instData.worldTransform[7] = y * (range+1.0f) - 25.0f;
			instData.diffuse[2] += 0.1f;
			for( int z=0; z<5; z++ )
			{
				instData.worldTransform[11] = z * (range+1.0f) - 25.0f;
				instData.diffuse[3] += 0.1f;
				m_data.push_back( instData );
			}
		}
	}
}

void LightRenderSystem::render()
{
	m_gfxBackend->getGfxWrapper()->renderLights( m_box, &m_data );
}
