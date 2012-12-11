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
		Input* inp =
			static_cast<Input*>(
			m_world->getComponentManager()->getComponent( p_entities[0],
			ComponentType::getTypeFor( ComponentType::Input ) ) );

		CameraInfo* cam = 
			static_cast<CameraInfo*>(
			m_world->getComponentManager()->getComponent( p_entities[0],
			ComponentType::getTypeFor( ComponentType::CameraInfo ) ) );

		if( cam != NULL )
		{
			double xp = m_inputManager->getControl(m_controlIdxs[1])->getStatus();
			double xn = m_inputManager->getControl(m_controlIdxs[2])->getStatus();
			double x = xp - xn;

			int rd = m_inputManager->getControl(m_controlIdxs[2])->getRawData();
			//cam->m_pos.x += x*500.0;
		}

		if( inp->m_bBtnPressed == true )
			int breakHere = 0;

		inp->m_bBtnPressed = true;
	}
}
