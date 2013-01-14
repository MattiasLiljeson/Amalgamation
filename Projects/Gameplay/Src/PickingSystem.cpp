#include "PickingSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include <PhysicsController.h>

PickingSystem::PickingSystem()
	: EntitySystem(SystemType::PickingSystem, 1, ComponentType::CameraInfo)
{
	m_rayID = -1;
}


PickingSystem::~PickingSystem()
{
}

void PickingSystem::initialize()
{
}

void PickingSystem::processEntities(const vector<Entity*>& p_entities)
{
	if (m_rayID < 0)
		createRay();

	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		CameraInfo* camInfo = static_cast<CameraInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// Retrieve initial info
		AglVector3 position = transform->getTranslation();
		AglQuaternion rotation = transform->getRotation();
		AglVector3 lookTarget = position+transform->getMatrix().GetForward();
		AglVector3 up = transform->getMatrix().GetUp();

		// Construct view matrix
		AglMatrix view = AglMatrix::createViewMatrix(position,
			lookTarget,
			up);

		AglMatrix proj = camInfo->m_projMat;

		AglMatrix inv = view*proj;
		inv = AglMatrix::inverse(inv);

		//Transform target location from screen space to world space
		AglVector4 targetNDC(0, 0, 1, 1);
		targetNDC.transform(inv);
		targetNDC /= targetNDC.w;
		AglVector3 target(targetNDC.x, targetNDC.y, targetNDC.z);
		AglVector3 dir = target - position;
		dir.normalize();

		setRay(m_rayID, position, dir);





		GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ));

		//gfx->getGfxWrapper()->
	}
}

void PickingSystem::createRay()
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	m_rayID = physX->getPhysicsController()->AddRay(AglVector3(0, 0, 0), AglVector3(0, 0, 1));
}
void PickingSystem::setRay(int p_index, AglVector3 p_o, AglVector3 p_dir)
{
	PhysicsSystem* physX = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));

	physX->getPhysicsController()->SetRay(p_o, p_dir, p_index);
}