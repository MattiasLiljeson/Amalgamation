#include "PortalCullingSystem.h"
#include "LevelPieceRoot.h"
#include "CameraInfo.h"
#include "Transform.h"
#include "BoundingBox.h"
#include "MeshOffsetTransform.h"

PortalCullingSystem::PortalCullingSystem()
	: EntitySystem( SystemType::PortalCullingSystem, 1,
	ComponentType::ComponentTypeIdx::LevelPieceRoot )
{	
	m_renderedChambers = 0;
	m_culledChambers = 0;
	m_collidedGate = NULL;
	m_collidedChamber = NULL;
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

	vector<Entity*> added;
	vector<Entity*> gates;

	Entity* collidedGateThisFrame = NULL;

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(p_entities[i]->getComponent(ComponentType::LevelPieceRoot));
		if (root->boundingSphere.radius > 0) //All chambers have larger than zero radius. Portals do not
		{
			float dist = AglVector3::length(m_cameraPos-root->boundingSphere.position) - root->boundingSphere.radius;
			if (root->boundingSphere.pointInSphere(m_cameraPos) && (p_entities[i] == m_collidedChamber || !m_collidedChamber))
			{
				bool hasThatSpecialSomeoneGate = false;
				for (unsigned int gate = 0; gate < root->connectedRootPieces.size(); gate++)
				{
					Entity* gateEntity = findRootPieceOrGate(root->connectedRootPieces[gate], p_entities);
					if (frustumGate(gateEntity))
					{
						//If the camera collides with the gate
						if (CameraPosInGate(gateEntity))
							collidedGateThisFrame = gateEntity;

						gates.push_back(gateEntity);

						if (m_collidedGate == gateEntity)
							hasThatSpecialSomeoneGate = true;
					}
				}
				if (!m_collidedGate || hasThatSpecialSomeoneGate)
				{
					m_renderedChambers++;
					root->shouldCull = false;
					added.push_back(p_entities[i]);
				}
				else
				{
					m_culledChambers++;
					root->shouldCull = true;
				}
			}
			else
			{
				m_culledChambers++;
				root->shouldCull = true;
			}
		}
	}

	if (collidedGateThisFrame) //Entered or stayed in a gate
		m_collidedChamber = NULL;
	else if (!collidedGateThisFrame && m_collidedGate) // Moved out of a gate
	{
		LevelPieceRoot* gate = static_cast<LevelPieceRoot*>(m_collidedGate->getComponent(ComponentType::LevelPieceRoot));
		Transform* gateTransform = static_cast<Transform*>(m_collidedGate->getComponent(ComponentType::Transform));
		MeshOffsetTransform* gateOffset = static_cast<MeshOffsetTransform*>(m_collidedGate->getComponent(ComponentType::MeshOffsetTransform));
		AglMatrix finalTransform = gateOffset->offset.inverse() * gateTransform->getMatrix();

		float side = AglVector3::dotProduct(m_cameraPos - finalTransform.GetTranslation(), finalTransform.GetForward());
		if (side > 0 || gate->connectedRootPieces[ChamberSide::CHAMBERSIDE_CHILD] < 0)
			m_collidedChamber = findRootPieceOrGate(gate->connectedRootPieces[ChamberSide::CHAMBERSIDE_PARENT], p_entities);
		else
			m_collidedChamber = findRootPieceOrGate(gate->connectedRootPieces[ChamberSide::CHAMBERSIDE_CHILD], p_entities);
	}

	m_collidedGate = collidedGateThisFrame;

	for (unsigned int i = 0; i < gates.size(); i++)
	{
		LevelPieceRoot* gate = static_cast<LevelPieceRoot*>(gates[i]->getComponent(ComponentType::LevelPieceRoot));
		for (unsigned int chamber = 0; chamber < gate->connectedRootPieces.size(); chamber++)
		{
			Entity* chamberEntiy = findRootPieceOrGate(gate->connectedRootPieces[chamber], p_entities);
			if (chamberEntiy && !IsAdded(chamberEntiy, added))
			{
				added.push_back(chamberEntiy);
				LevelPieceRoot* chamberPiece = static_cast<LevelPieceRoot*>(chamberEntiy->getComponent(ComponentType::LevelPieceRoot));
				m_renderedChambers++;
				m_culledChambers--;
				chamberPiece->shouldCull = false;
			}
		}
	}

	m_collidesWithGate = m_collidedGate != NULL;
	m_collidesWithChamber = m_collidedChamber != NULL;


	/*while (checkNeighbours.size() > 0)
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
	}*/
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

Entity* PortalCullingSystem::findRootPieceOrGate(int p_id, const vector<Entity*>& p_list)
{
	for (unsigned int i = 0; i < p_list.size(); i++)
	{
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(p_list[i]->getComponent(ComponentType::LevelPieceRoot));
		if (p_id == root->pieceId)
			return p_list[i];
	}
	return NULL;
}
bool PortalCullingSystem::frustumGate(Entity* p_gate)
{
	Transform* transform = static_cast<Transform*>(
		p_gate->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	BoundingBox* bb = static_cast<BoundingBox*>(
		p_gate->getComponent( ComponentType::ComponentTypeIdx::BoundingBox ) );

	if (!bb)
		return true;

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
			return false;
		}
	}
	return true;
}

//Returns true if the box is completely outside the plane
bool PortalCullingSystem::BoxPlane(const AglOBB& p_box, const AglVector4& p_plane)
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
bool PortalCullingSystem::IsAdded(Entity* p_chamber, const vector<Entity*>& p_list)
{
	for (unsigned int i = 0; i < p_list.size(); i++)
	{
		if (p_chamber == p_list[i])
			return true;
	}
	return false;
}
bool PortalCullingSystem::CameraPosInGate(Entity* p_gate)
{
	Transform* transform = static_cast<Transform*>(
		p_gate->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	BoundingBox* bb = static_cast<BoundingBox*>(
		p_gate->getComponent( ComponentType::ComponentTypeIdx::BoundingBox ) );

	if (!bb)
		return true;

	AglMatrix rbtransform;
	AglMatrix::componentsToMatrix(rbtransform, AglVector3(1, 1, 1), transform->getRotation(), transform->getTranslation());

	AglOBB box = bb->box;
	AglVector3 pos = box.world.GetTranslation();

	pos.transform(transform->getMatrix());
	box.world *= rbtransform;
	box.world.SetTranslation(pos);
	box.size *= transform->getScale();


	pos = m_cameraPos;
	pos.transform(box.world.inverse());

	if (pos.x > -box.size.x)
		if (pos.x < box.size.x)
			if (pos.y > -box.size.y)
				if (pos.y < box.size.y)
					if (pos.z > -box.size.z)
						if (pos.z < box.size.z)
							return true;
	return false;
}