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
		
		// Pre-alloc for speed
		//InstanceVertex vert;

		//HACK: continues in loop below 
		for( int i=0; i<p_entities.size(); i++ )
		{
			RenderInfo* renderInfo = static_cast<RenderInfo*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

			if( renderInfo->m_meshId == -1)
			{
				// Don't render instances that hasn't got a mesh 
				continue;
			}

			Transform* transform = static_cast<Transform*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

			if( transform == NULL )
			{
				// Don't render instances that hasn't got a transformation
				continue;
			}

			// resize vector if the mesh id is outside of the vectors size
			if( m_instanceLists.size() <= renderInfo->m_meshId )
			{
				m_instanceLists.resize( renderInfo->m_meshId + 1 );
			}

			// Finally, add the entity to the instance vector
			m_instanceLists[renderInfo->m_meshId].push_back( transform->getInstanceVertexRef() );
		}
		for( int meshIdx=0; meshIdx<m_instanceLists.size(); meshIdx++ )
		{
			// Batch render all entities that share the same mesh
			gfxWrapper->renderMesh( meshIdx, &m_instanceLists[meshIdx] ); // process a mesh
		}

		gfxWrapper->finalizeFrame();			  // finalize, draw to back buffer        
		AntTweakBarWrapper::getInstance()->render();
		gfxWrapper->flipBackBuffer();           // flip buffers
		
		// WOW! for each loop in C++!
		for( int i=0; i<m_instanceLists.size(); i++ )
		{
			m_instanceLists[i].clear();
		}
	}
}
