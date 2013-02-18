#include "GraphicsBackendSystem.h"
#include "CullingSystem.h"
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

CullingSystem::CullingSystem()
	: EntitySystem( SystemType::CullingSystem, 1,
		ComponentType::ComponentTypeIdx::RenderInfo )
{	
	m_rendered = 0;
	m_culled = 0;
	m_culledFraction = 0.0f;
}

CullingSystem::~CullingSystem()
{

}

void CullingSystem::initialize()
{
}

void CullingSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_culled = 0;
	m_rendered = 0;
	calcCameraPlanes();

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		RenderInfo* renderInfo = static_cast<RenderInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::RenderInfo ) );

		//Perform some culling checks
		if (renderInfo->m_shouldBeRendered)
		{
			if (shouldCull(p_entities[i]))
			{
				renderInfo->m_shouldBeCulled = true;
				m_culled++;
				continue;
			}
			else
			{
				renderInfo->m_shouldBeCulled = false;
				m_rendered++;
			}
		}
	}
	m_culledFraction = m_culled / (float)(m_culled+m_rendered);
}

bool CullingSystem::shouldCull(Entity* p_entity)
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
void CullingSystem::calcCameraPlanes()
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
bool CullingSystem::BoxPlane(const AglOBB& p_box, const AglVector4& p_plane)
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
