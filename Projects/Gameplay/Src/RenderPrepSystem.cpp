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
		GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

		gfxWrapper->clearRenderTargets();	      // clear render targets used           
		gfxWrapper->beginFrame();				  // prepare frame, set drawing to MRT   
		
		vector< vector<InstanceVertex> > instanceLists;
		for( int i=0; i<p_entities.size(); i++ )
		{
			RenderInfo* renderInfo = static_cast<RenderInfo*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

			if( renderInfo->m_meshId == -1)
			{
				// Create a cube
				renderInfo->m_meshId = gfxWrapper->createMesh( "P_cube" );
			}

			Transform* transform = static_cast<Transform*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

			AglMatrix mat = AglMatrix::identityMatrix();
			if( transform != NULL )
			{
				mat = *transform->getMatrix();
			}

			InstanceVertex vert;
			for( int i=0; i<16; i++ )
			{
				vert.worldTransform[i] = mat[i];
			}

			if( instanceLists.size() <= renderInfo->m_meshId )
				instanceLists.resize( renderInfo->m_meshId + 1 );

			instanceLists[renderInfo->m_meshId].push_back( vert );
		}
		for( int meshIdx=0; meshIdx<instanceLists.size(); meshIdx++)
		{
			gfxWrapper->renderMesh( meshIdx, &instanceLists[meshIdx] ); // process a mesh
		}

		gfxWrapper->finalizeFrame();			  // finalize, draw to backbuffer        
		AntTweakBarWrapper::getInstance()->render();
		gfxWrapper->flipBackBuffer();           // flip buffers	
	}
}
