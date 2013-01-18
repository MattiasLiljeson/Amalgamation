#include "LightRenderSystem.h"

#include "GraphicsBackendSystem.h"
#include <BufferFactory.h>
#include <GraphicsWrapper.h>
#include <InstanceData.h>
#include <Mesh.h>


LightRenderSystem::LightRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::LightRenderSystem )
{
	m_gfxBackend = p_gfxBackend;

	m_box = NULL;
}


LightRenderSystem::~LightRenderSystem()
{
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
	//gfxWrapper->renderLights( NULL, NULL );

	InstanceData instData;
	instData.worldTransform[0] = 1.0f;
	instData.worldTransform[1] = 0.0f;
	instData.worldTransform[2] = 0.0f;
	instData.worldTransform[3] = 0.0f;

	instData.worldTransform[4] = 0.0f;
	instData.worldTransform[5] = 1.0f;
	instData.worldTransform[6] = 0.0f;
	instData.worldTransform[7] = 0.0f;

	instData.worldTransform[8] = 0.0f;
	instData.worldTransform[9] = 0.0f;
	instData.worldTransform[10] = 1.0f;
	instData.worldTransform[11] = 0.0f;

	instData.worldTransform[12] = 0.0f;
	instData.worldTransform[13] = 0.0f;
	instData.worldTransform[14] = 0.0f;
	instData.worldTransform[15] = 1/10.0f;

	vector<InstanceData> instDatas;
	instDatas.push_back( instData );
	gfxWrapper->renderLights( m_box, &instDatas );
	 
	gfxWrapper->endLightPass();
}
