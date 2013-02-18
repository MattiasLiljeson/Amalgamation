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
		RenderInfo* renderInfo = static_cast<RenderInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

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

		//Check if the object should be drawn
		RenderInfo* ri = static_cast<RenderInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo) );

		if (!ri->m_shouldBeRendered)
			continue;

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

		// Finally, add the entity to the instance vector
		InstanceData instanceData = transform->getInstanceDataRef();
		MaterialInfo matInfo = m_gfxBackend->getGfxWrapper()->getMaterialInfoFromMeshID(
			renderInfo->m_meshId);
		auto gradient = static_cast<GradientComponent*>(p_entities[i]->getComponent(ComponentType::Gradient));
		if(gradient != NULL){ 
			matInfo.setGradientLayer(1,gradient->m_color.playerSmall);
			matInfo.setGradientLayer(2,gradient->m_color.playerBig);
		}
		instanceData.setGradientColor( matInfo.getGradientColors() );
		instanceData.setNumberOfActiveGradientLayers( matInfo.numberOfLayers );
		
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


	//Bounding Sphere check
	/*BoundingSphere* bs = static_cast<BoundingSphere*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::BoundingSphere ) );

	//Use offset to get correct bounding sphere - NOT USED RIGHT NOW. Might cause artifacts
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::MeshOffsetTransform ) );

	if (!bs)
		return false;

	AglBoundingSphere sphere = bs->sphere;
	sphere.position.transform(transform->getMatrix());
	AglVector3 scale = transform->getScale();

	sphere.radius *= max(scale.x, max(scale.y, scale.z));

	for (unsigned int i = 0; i < 6; i++)
	{
		float val = m_cameraPlanes[i].x * sphere.position.x + 
					m_cameraPlanes[i].y * sphere.position.y +
					m_cameraPlanes[i].z * sphere.position.z +
					m_cameraPlanes[i].w * 1;
		if (val + sphere.radius < 0 )
		{
			return true;
		}

	}*/

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
