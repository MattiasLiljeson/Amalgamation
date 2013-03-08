#include "PortalCullingSystem.h"
#include "LevelPieceRoot.h"
#include "CameraInfo.h"
#include "Transform.h"

PortalCullingSystem::PortalCullingSystem()
	: EntitySystem( SystemType::PortalCullingSystem, 1,
	ComponentType::ComponentTypeIdx::LevelPieceRoot )
{	
	m_renderedChambers = 0;
	m_culledChambers = 0;
}

PortalCullingSystem::~PortalCullingSystem()
{

}

void PortalCullingSystem::initialize()
{
}

void PortalCullingSystem::processEntities( const vector<Entity*>& p_entities )
{
	//Get the camera position
	calcCameraPlanes();

	EntityManager* entitymanager = m_world->getEntityManager();
	Entity* ship = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MyShip);
	AglVector3 shipPos;
	if (ship)
	{
		Transform* transform2 = static_cast<Transform*>(
			ship->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		shipPos = transform2->getTranslation();
	}


	m_renderedChambers = 0;
	m_culledChambers = 0;

	vector<Entity*> checkNeighbours;
	vector<int> added;
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(p_entities[i]->getComponent(ComponentType::LevelPieceRoot));
		if (root->boundingSphere.radius > 0) //All chambers have larger than zero radius. Portals do not
		{
			float dist = AglVector3::length(m_cameraPos-root->boundingSphere.position) - root->boundingSphere.radius;
			if (root->boundingSphere.pointInSphere(m_cameraPos))
			{
				m_renderedChambers++;
				root->shouldCull = false;
				checkNeighbours.push_back(p_entities[i]);
				added.push_back(i);
			}
			else
			{
				m_culledChambers++;
				root->shouldCull = true;
			}
		}
	}

	while (checkNeighbours.size() > 0)
	{
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(checkNeighbours.back()->getComponent(ComponentType::LevelPieceRoot));
		for (unsigned int i = 0; i < p_entities.size(); i++)
		{
			LevelPieceRoot* root2 = static_cast<LevelPieceRoot*>(p_entities[i]->getComponent(ComponentType::LevelPieceRoot));

			if (root2->boundingSphere.radius > 0)
			{
				bool shouldAdd = true;
				for (unsigned int j = 0; j < added.size(); j++)
				{
					if (added[j] == i)
						shouldAdd = false;
				}

				if (shouldAdd)
				{
					//Check frustum collision
					AglBoundingSphere sphere = root2->boundingSphere;
					for (unsigned int plane = 0; plane < 6; plane++)
					{
						float val = m_cameraPlanes[plane].x * sphere.position.x + 
							m_cameraPlanes[plane].y * sphere.position.y +
							m_cameraPlanes[plane].z * sphere.position.z +
							m_cameraPlanes[plane].w * 1;
						if (val + sphere.radius < 0 )
						{
							shouldAdd = false;
						}
					}
					if (shouldAdd)
					{
						added.push_back(i);
						root2->shouldCull = false;
						m_culledChambers--;
						m_renderedChambers++;
					}
				}
			}
		}
		checkNeighbours.pop_back();
	}
}

void PortalCullingSystem::calcCameraPlanes()
{
	EntityManager* entitymanager = m_world->getEntityManager();
	Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);

	CameraInfo* info = static_cast<CameraInfo*>(cam->getComponent(ComponentType::CameraInfo));

	Transform* transform = static_cast<Transform*>(
		cam->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	m_cameraPos = transform->getTranslation();

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