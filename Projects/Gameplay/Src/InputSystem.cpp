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
	XInputFetcher* xif = new XInputFetcher();
	MessageLoopFetcher* milf = new MessageLoopFetcher();
	m_inputManager = new InputManager( milf, xif);

	InputControlFactory icf;
	Control* mbl = icf.create360controllerDigital( InputHelper::BTN_B );
	int mblIdx = m_inputManager->addControl(mbl);
	m_controlIdxs.push_back(mblIdx);
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

		if( inp->m_bBtnPressed == true )
			int breakHere = 0;

		inp->m_bBtnPressed = true;
	}
}
