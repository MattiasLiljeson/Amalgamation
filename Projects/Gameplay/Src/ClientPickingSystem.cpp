#include "ClientPickingSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "CameraInfo.h"
#include <PhysicsController.h>
#include <Cursor.h>
#include "RayPacket.h"
#include "Control.h"
#include "SimpleEventPacket.h"

ClientPickingSystem::ClientPickingSystem(TcpClient* p_client)
	: EntitySystem(SystemType::ClientPickingSystem, 2, ComponentType::CameraInfo, ComponentType::PickComponent)
{
	m_client = p_client;
}


ClientPickingSystem::~ClientPickingSystem()
{
}

void ClientPickingSystem::initialize()
{
}

void ClientPickingSystem::processEntities(const vector<Entity*>& p_entities)
{
	/*InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::InputBackendSystem));
	Control* leftBtnControl = input->getControlByEnum(InputHelper::KEY_K);
	double pressed = leftBtnControl->getDelta();

	if (pressed <= 0)
		return;*/

	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		PickComponent* ray = static_cast<PickComponent*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::PickComponent ) );

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


		//view = View Matrix of Camera
		AglMatrix proj = camInfo->m_projMat;

		AglMatrix inv = view*proj;
		inv = AglMatrix::inverse(inv);

		//Transform target location from screen space to world space
		InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(
			SystemType::InputBackendSystem ));

		Control* ctrl = input->getControlByEnum(
			InputHelper::MouseButtons_RIGHT);

		Control* ctrl2 = input->getControlByEnum(
			InputHelper::MouseButtons_LEFT);

		double x = input->getCursor()->getX();
		double y = -input->getCursor()->getY();
		AglVector4 targetNDC( (float)x, (float)y, 1.0f, 1.0f );
		targetNDC.transform(inv);
		targetNDC /= targetNDC.w;
		AglVector3 target(targetNDC.x, targetNDC.y, targetNDC.z);
		AglVector3 dir = target - position;
		dir.normalize();

		RayPacket rp;
		rp.o = position;
		rp.d = dir;
		m_client->sendPacket(rp.pack());

		if (ctrl->getDelta() > 0) //Start picking
		{
			SimpleEventPacket packet;
			packet.type = SimpleEventType::ACTIVATE_PICK;
			m_client->sendPacket( packet.pack() );
		}
		else if (ctrl->getDelta() < 0) //Stop picking
		{
			SimpleEventPacket packet;
			packet.type = SimpleEventType::DEACTIVATE_PICK;
			m_client->sendPacket( packet.pack() );
		}
		if (ctrl2->getDelta() > 0) //Release current module
		{
			SimpleEventPacket packet;
			packet.type = SimpleEventType::RELEASE_PICK;
			m_client->sendPacket( packet.pack() );
		}
	}
}