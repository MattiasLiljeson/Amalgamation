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

		//HACK: breaks in loop below 
		for( int i=0; i<p_entities.size(); i++ )
		{
			RenderInfo* renderInfo = static_cast<RenderInfo*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

			if( renderInfo->m_meshId == -1)
			{
				// Don't render instances that hasn't got a mesh 
				break;
			}

			Transform* transform = static_cast<Transform*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

			if( transform == NULL )
			{
				// Don't render instances that hasn't got a transformation
				break;
			}

			AglMatrix mat = *transform->getMatrix();
			// The renderer wants col-major matrices, therefore transpose
			mat = AglMatrix::transpose( mat );
			
			// Create instance vertex for this entity 
			InstanceVertex vert;
			for( int i=0; i<16; i++ )
			{
				vert.worldTransform[i] = mat[i];
			}

			// resize vector if the mesh id is outside of the vectors size
			if( instanceLists.size() <= renderInfo->m_meshId )
				instanceLists.resize( renderInfo->m_meshId + 1 );

			// Finally, add the entity to the instance vector
			instanceLists[renderInfo->m_meshId].push_back( vert );
		}
		for( int meshIdx=0; meshIdx<instanceLists.size(); meshIdx++ )
		{
			// Batch render all entities that share the same mesh
			gfxWrapper->renderMesh( meshIdx, &instanceLists[meshIdx] ); // process a mesh
		}

		gfxWrapper->finalizeFrame();			  // finalize, draw to back buffer        
		AntTweakBarWrapper::getInstance()->render();
		gfxWrapper->flipBackBuffer();           // flip buffers	
	}
}
