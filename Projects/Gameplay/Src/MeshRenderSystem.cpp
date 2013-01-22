#include "GraphicsBackendSystem.h"
#include "MeshRenderSystem.h"
#include "RenderInfo.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglVector3.h>
#include <ComponentType.h>
#include <DebugUtil.h>
#include <GraphicsWrapper.h>
#include <InstanceData.h>
#include <TextureParser.h>

MeshRenderSystem::MeshRenderSystem(  GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::RenderPrepSystem, 1,
		ComponentType::ComponentTypeIdx::RenderInfo )
{	
	m_gfxBackend = p_gfxBackend;
}

MeshRenderSystem::~MeshRenderSystem()
{

}

void MeshRenderSystem::initialize()
{
}

void MeshRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

	gfxWrapper->clearRenderTargets();	      // clear render targets used           
	gfxWrapper->beginFrame();				  // prepare frame, set drawing to MRT

	//NOTE: continues in loop below 
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		RenderInfo* renderInfo = static_cast<RenderInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

		// Don't render instances that hasn't got a mesh 
		if( renderInfo->m_meshId == -1)
		{
			continue;
		}

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// Don't render instances that hasn't got a transformation
		if( transform == NULL )
		{
			continue;
		}

		// resize vector if the mesh id is outside of the vectors size
		if( m_instanceLists.size() <= static_cast<unsigned int>(renderInfo->m_meshId) )
		{
			m_instanceLists.resize( renderInfo->m_meshId + 1 );
		}

		// Finally, add the entity to the instance vector
		m_instanceLists[renderInfo->m_meshId].push_back( transform->getInstanceDataRef() );
	}
		
	for(unsigned int meshIdx=0; meshIdx<m_instanceLists.size(); meshIdx++ )
	{
		// Batch render all entities that share the same mesh
		if (!m_instanceLists[meshIdx].empty())
		{
			gfxWrapper->renderMesh( meshIdx, &m_instanceLists[meshIdx] ); // process a mesh
		}
	}
	
	// Cleanup
	for(unsigned int i=0; i<m_instanceLists.size(); i++ )
	{
		m_instanceLists[i].clear();
	}
}
