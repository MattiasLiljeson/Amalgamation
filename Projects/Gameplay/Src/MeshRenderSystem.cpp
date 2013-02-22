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
#include "CameraSystem.h"
#include "BoundingSphere.h"
#include "CameraInfo.h"
#include "MaterialInfo.h"
#include "GradientComponent.h"
#include "BoundingBox.h"
#include "InputBackendSystem.h"
#include "ModuleHelper.h"
#include "ShipModule.h"

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
	for(unsigned int i=0; i<m_instanceListsTess.size(); i++ ){
		m_instanceListsTess[i].clear();
	}

	for(unsigned int i=0; i<m_boneMatrices.size(); i++ ){
		m_boneMatrices[i].clear();
	}

//	DEBUGPRINT(( ("\nEntities rendered: "+toString(p_entities.size())).c_str() ));
	//NOTE: continues in loop below 
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		vector<ComponentType::ComponentTypeIdx> types = p_entities[i]->getComponentEnums();
		
		RenderInfo* renderInfo = getRenderInfo(p_entities[i]);

		// Don't render instances that hasn't got a mesh...
		// NOTE: (Johan) ...or if it's not supposed to render!
		if( renderInfo->m_meshId == -1 || renderInfo->m_shouldBeRendered == false )
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
			m_instanceListsTess.resize(renderInfo->m_meshId + 1);
			m_boneMatrices.resize( renderInfo->m_meshId + 1 );
		}
		
		if (!renderInfo->m_shouldBeRendered)
			continue;

		if (renderInfo->m_shouldBeCulled)
			continue;


		SkeletalAnimation* skelAnim = static_cast<SkeletalAnimation*>
			(p_entities[i]->getComponent(ComponentType::SkeletalAnimation));
		AglSkeleton* skeleton = NULL;
		unsigned int jointCount = -1;
		if (skelAnim)
		{
			skeleton = skelAnim->m_scene->getSkeleton(0);
			jointCount = skeleton->getHeader().jointCount;
		}

		// Finally, add the entity to the instance vector
		InstanceData instanceData = transform->getInstanceDataRef();
		fillInstanceData(&instanceData,p_entities[i],renderInfo, jointCount);

		if (renderInfo->m_shouldBeTesselated)
			m_instanceListsTess[renderInfo->m_meshId].push_back( instanceData );
		else
			m_instanceLists[renderInfo->m_meshId].push_back( instanceData );

		//Find animation transforms
		if (skelAnim)
		{
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

void MeshRenderSystem::fillInstanceData(InstanceData* p_data, Entity* p_entity, 
										RenderInfo* p_renderInfo, int p_boneCount){
	MaterialInfo matInfo;

	p_data->setNumberOfActiveBones(p_boneCount);

	// Try and get the gradient component
	auto gradient = static_cast<GradientComponent*>(p_entity->getComponent(
		ComponentType::Gradient));
	if(gradient != NULL){ 

		// Set all the values needed
		matInfo = m_gfxBackend->getGfxWrapper()->getMaterialInfoFromMeshID(
			p_renderInfo->m_meshId);
		matInfo.setGradientLayer(1,gradient->m_color.layerOne);
		matInfo.setGradientLayer(2,gradient->m_color.layerTwo);
		p_data->setNumberOfActiveGradientLayers( matInfo.numberOfLayers );
	}
	// If none was found check why
	else{
		// Assume its a valid Ship Module
		ShipModule* shipModule = static_cast<ShipModule*>(m_world->
			getComponentManager()->getComponent(p_entity,ComponentType::ShipModule));

		if(shipModule != NULL && shipModule->m_parentEntity > -1){
			
			Entity* parentShip = m_world->getEntity(shipModule->m_parentEntity);
			ModuleHelper::FindParentShip(m_world,&parentShip, shipModule);

			if(parentShip != NULL){
				RenderInfo* parentShipRenderInfo = getRenderInfo(parentShip);
				matInfo = m_gfxBackend->getGfxWrapper()->getMaterialInfoFromMeshID(
					parentShipRenderInfo->m_meshId);

				auto gradient = static_cast<GradientComponent*>(parentShip->getComponent(
					ComponentType::Gradient));

				matInfo.setGradientLayer(1,gradient->m_color.layerOne);
				matInfo.setGradientLayer(2,gradient->m_color.layerTwo);
				p_data->setNumberOfActiveGradientLayers( matInfo.numberOfLayers );
			}
		}
		// If not a Ship Module set values to default
		else{
			matInfo.setGradientLayer(1, AglVector4(1,1,1,1));
			matInfo.setGradientLayer(2, AglVector4(1,1,1,1));
			p_data->setNumberOfActiveGradientLayers( 1 );
		}
	}	

	p_data->setGradientColor( matInfo.getGradientColors() );
}

void MeshRenderSystem::render()
{
	for(unsigned int meshIdx=0; meshIdx<m_instanceLists.size(); meshIdx++ ){
		// Batch render all entities that share the same mesh
		if (!m_instanceLists[meshIdx].empty())
		{
			m_gfxBackend->getGfxWrapper()->renderMesh( meshIdx, 
				&m_instanceLists[meshIdx], &m_boneMatrices[meshIdx], false); // process a mesh
		}
		if (!m_instanceListsTess[meshIdx].empty())
		{
			m_gfxBackend->getGfxWrapper()->renderMesh( meshIdx, 
				&m_instanceListsTess[meshIdx], &m_boneMatrices[meshIdx], true); // process a tesselated mesh
		}
	}
}

RenderInfo* MeshRenderSystem::getRenderInfo( Entity* p_entity )
{
	return static_cast<RenderInfo*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );
}
