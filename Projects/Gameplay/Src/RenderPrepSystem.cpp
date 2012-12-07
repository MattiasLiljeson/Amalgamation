#include "RenderPrepSystem.h"

RenderPrepSystem::RenderPrepSystem(  GraphicsBackendSystem* p_gfxBackend  ) : EntitySystem( SystemType::RenderPrepSystem, 1,
								   ComponentType::ComponentTypeIdx::RenderInfo )
{	
	m_gfxBackend = p_gfxBackend;
}

RenderPrepSystem::~RenderPrepSystem()
{
}

void RenderPrepSystem::initialize()
{
}

void RenderPrepSystem::processEntities( const vector<Entity*>& p_entities )
{
	MSG msg = {0};
	if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	else
	{
		// * Deferred base system *            1
		GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

		gfxWrapper->clearRenderTargets();	      // clear render targets used           
		gfxWrapper->beginFrame();				  // prepare frame, set drawing to MRT   

		// * Render system *                   N
		for( int i=0; i<p_entities.size(); i++ )
		{
			Component* comp = m_world->getComponentManager()->getComponent( p_entities[i],
				ComponentType::ComponentTypeIdx::RenderInfo);

			RenderInfo* renderInfo = static_cast<RenderInfo*>(comp);
			int instId = renderInfo->m_instanceId;
			if( instId != -1)
			{
				gfxWrapper->renderMesh( instId ); // process a mesh
			}
			else
			{
				// Create a cube
				renderInfo->m_instanceId = gfxWrapper->createMesh( "P_cube", 0 );
			}
		}

		// * Deferred finalize system *        1
		gfxWrapper->finalizeFrame();			  // finalize, draw to backbuffer        
		AntTweakBarWrapper::getInstance()->render();
		gfxWrapper->flipBackBuffer();           // flip buffers	
		static int korv = 0;
		korv++;
	}
}
