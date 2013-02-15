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
#include "SkeletalAnimation.h"
#include "MeshOffsetTransform.h"
#include <MaterialInfo.h>

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
	// Cleanup
	for(unsigned int i=0; i<m_instanceLists.size(); i++ ){
		m_instanceLists[i].clear();
	}
	for(unsigned int i=0; i<m_boneMatrices.size(); i++ ){
		m_boneMatrices[i].clear();
	}

//	DEBUGPRINT(( ("\nEntities rendered: "+toString(p_entities.size())).c_str() ));
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
			m_boneMatrices.resize( renderInfo->m_meshId + 1 );
		}

		// Finally, add the entity to the instance vector
		InstanceData instanceData = transform->getInstanceDataRef();
		instanceData.setGradientColor(
			m_gfxBackend->getGfxWrapper()->getMaterialInfoFromMeshID(
			renderInfo->m_meshId).getGradientColors());
		instanceData.setNumberOfActiveGradientLayers(
			m_gfxBackend->getGfxWrapper()->getMaterialInfoFromMeshID(
			renderInfo->m_meshId).numberOfLayers
			);
		
		m_instanceLists[renderInfo->m_meshId].push_back( instanceData );

		//Find animation transforms
		SkeletalAnimation* skelAnim = static_cast<SkeletalAnimation*>
			(p_entities[i]->getComponent(ComponentType::SkeletalAnimation));

		if (skelAnim)
		{
			AglSkeleton* skeleton = skelAnim->m_scene->getSkeleton(0);
			unsigned int jointCount = skeleton->getHeader().jointCount;
			for (unsigned int i = 0; i < jointCount; i++)
			{
				skelAnim->m_scene->setTime(skelAnim->m_time);
				AglMatrix m = skeleton->getInverseBindMatrix(i) * skeleton->getGlobalTransform(i);

				m *= skelAnim->m_offset;

				m_boneMatrices[renderInfo->m_meshId].push_back(m);
			}
		}
	}
}

void MeshRenderSystem::render()
{
	for(unsigned int meshIdx=0; meshIdx<m_instanceLists.size(); meshIdx++ ){
		// Batch render all entities that share the same mesh
		if (!m_instanceLists[meshIdx].empty())
		{
			m_gfxBackend->getGfxWrapper()->renderMesh( meshIdx, 
				&m_instanceLists[meshIdx], &m_boneMatrices[meshIdx]); // process a mesh
		}
	}
}
