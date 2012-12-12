#include "InputSystem.h"

InputSystem::InputSystem(void)
	: EntitySystem( SystemType::InputSystem, 1, ComponentType::Input )
{
}

InputSystem::~InputSystem(void)
{
}

void InputSystem::initialize()
{
	//XInputFetcher* xif = new XInputFetcher();
	MessageLoopFetcher* milf = new MessageLoopFetcher( false );
	m_inputManager = new InputManager( milf, /*xif*/ NULL );

	InputControlFactory icf;
	Control* gamepadB = icf.create360controllerDigital( InputHelper::BTN_B );
	int idx = m_inputManager->addControl( gamepadB );
	m_controlIdxs.push_back(idx);

	Control* mouseXP = icf.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	idx = m_inputManager->addControl( mouseXP );
	m_controlIdxs.push_back(idx);

	Control* mouseXN = icf.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	idx = m_inputManager->addControl( mouseXN );
	m_controlIdxs.push_back(idx);

	Control* mouseYP = icf.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	idx = m_inputManager->addControl( mouseYP );
	m_controlIdxs.push_back(idx);

	Control* mouseYN = icf.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	idx = m_inputManager->addControl( mouseYN );
	m_controlIdxs.push_back(idx);
}

void InputSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_inputManager->update();

	double pressness = m_inputManager->getControl(m_controlIdxs[0])->getStatus();
	bool pressed = false; 
	if(pressness == 1.0f)
		pressed = true;

	if( p_entities.size() > 0 )
	{
		Input* inp = static_cast<Input*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Input ) );

		CameraInfo* cam = static_cast<CameraInfo*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* trans = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		if( cam != NULL )
		{
			double x = 0.0, y = 0.0;
			x += m_inputManager->getControl(m_controlIdxs[1])->getStatus();
			x -= m_inputManager->getControl(m_controlIdxs[2])->getStatus();
			y += m_inputManager->getControl(m_controlIdxs[3])->getStatus();
			y -= m_inputManager->getControl(m_controlIdxs[4])->getStatus();

			AglVector3* pos = trans->getTranslation();
			double sensitivityMult = 1000.0;
			pos->x -= x*sensitivityMult;
			pos->y -= y*sensitivityMult;
		}
	}
}
