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
#include "ModuleHelper.h"
#include "ShipModule.h"

MeshRenderSystem::MeshRenderSystem(  GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::RenderPrepSystem, 1,
		ComponentType::ComponentTypeIdx::RenderInfo )
{	
	m_gfxBackend = p_gfxBackend;
	
	m_rendered = 0;
	m_culled = 0;
	m_culledFraction = 0.0f;
}

MeshRenderSystem::~MeshRenderSystem()
{

}

void MeshRenderSystem::initialize()
{
}

void MeshRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_culled = 0;
	m_rendered = 0;
	calcCameraPlanes();

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
			m_boneMatrices.resize( renderInfo->m_meshId + 1 );
		}

		//Perform some culling checks
		if (shouldCull(p_entities[i]))
		{
			m_culled++;
			continue;
		}
		else
		{
			m_rendered++;
		}

		InstanceData instanceData = transform->getInstanceDataRef();
		fillInstanceData(&instanceData,p_entities[i],renderInfo);
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
	m_culledFraction = m_culled / (float)(m_culled+m_rendered);
}

void MeshRenderSystem::fillInstanceData(InstanceData* p_data, Entity* p_entity, 
										RenderInfo* p_renderInfo){
	MaterialInfo matInfo;
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
			ModuleHelper::FindParentShip(m_world,&parentShip, &shipModule);

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
				&m_instanceLists[meshIdx], &m_boneMatrices[meshIdx]); // process a mesh
		}
	}
}

bool MeshRenderSystem::shouldCull(Entity* p_entity)
{
	Transform* transform = static_cast<Transform*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	//Bounding Box check
	BoundingBox* bb = static_cast<BoundingBox*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::BoundingBox ) );

	//Use offset to get correct bounding sphere - NOT USED RIGHT NOW. Might cause artifacts
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::MeshOffsetTransform ) );

	if (!bb)
		return false;

	AglMatrix rbtransform;
	AglMatrix::componentsToMatrix(rbtransform, AglVector3(1, 1, 1), transform->getRotation(), transform->getTranslation());
	
	AglOBB box = bb->box;
	AglVector3 pos = box.world.GetTranslation();
	
	pos.transform(transform->getMatrix());
	box.world *= rbtransform;
	box.world.SetTranslation(pos);
	box.size *= transform->getScale();

	for (unsigned int i = 0; i < 6; i++)
	{
		if (BoxPlane(box, m_cameraPlanes[i]))
		{
			return true;
		}

	}
	return false;
}
void MeshRenderSystem::calcCameraPlanes()
{
	EntityManager* entitymanager = m_world->getEntityManager();
	Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);

	CameraInfo* info = static_cast<CameraInfo*>(cam->getComponent(ComponentType::CameraInfo));

	Transform* transform = static_cast<Transform*>(
		cam->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	AglVector3 position = transform->getTranslation();
	AglQuaternion rotation = transform->getRotation();
	AglVector3 lookTarget = position+transform->getMatrix().GetForward();
	AglVector3 up = transform->getMatrix().GetUp();

	AglMatrix view = AglMatrix::createViewMatrix(position,
		lookTarget,
		up);

	AglMatrix viewProj = view * info->m_projMat;

	m_cameraPlanes[0] = viewProj.getColumn(3)+viewProj.getColumn(0); //LEFT
	m_cameraPlanes[1] = viewProj.getColumn(3)-viewProj.getColumn(0); //RIGHT
	m_cameraPlanes[2] = viewProj.getColumn(3)-viewProj.getColumn(1); //TOP
	m_cameraPlanes[3] = viewProj.getColumn(3)+viewProj.getColumn(1); //BOTTOM
	m_cameraPlanes[4] = viewProj.getColumn(2);						 //NEAR
	m_cameraPlanes[5] = viewProj.getColumn(3)-viewProj.getColumn(2); //FAR

	for (unsigned int i = 0; i < 6; i++)
	{
		float l = sqrt(m_cameraPlanes[i].x * m_cameraPlanes[i].x +
				  m_cameraPlanes[i].y * m_cameraPlanes[i].y + 
				  m_cameraPlanes[i].z * m_cameraPlanes[i].z);

		m_cameraPlanes[i].x /= l;
		m_cameraPlanes[i].y /= l;
		m_cameraPlanes[i].z /= l;
		m_cameraPlanes[i].w /= l;
	}
}

//Returns true if the box is completely outside the plane
bool MeshRenderSystem::BoxPlane(const AglOBB& p_box, const AglVector4& p_plane)
{
	AglVector3 h = p_box.size * 0.5f;
	AglVector3 n = AglVector3(p_plane.x, p_plane.y, p_plane.z);
	float ex = h.x*abs(AglVector3::dotProduct(n, p_box.world.GetRight())); 
	float ey = h.y*abs(AglVector3::dotProduct(n, p_box.world.GetUp())); 
	float ez = h.z*abs(AglVector3::dotProduct(n, p_box.world.GetForward())); 
	
	float e = ex + ey + ez;
	float s = AglVector3::dotProduct(p_box.world.GetTranslation(), n)+p_plane.w;
	
	return s + e < 0;
}

RenderInfo* MeshRenderSystem::getRenderInfo( Entity* p_entity )
{
	return static_cast<RenderInfo*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );
}
