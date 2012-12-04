#include "InputSystem.h"

InputSystem::InputSystem(void)
	: EntitySystem( SystemType::InputSystem )
{
}


InputSystem::~InputSystem(void)
{
}

void InputSystem::initialize()
{
	XInputFetcher* xif = new XInputFetcher();
	MessageLoopFetcher* milf = new MessageLoopFetcher();
	m_manager = new InputManager( milf, xif);

	InputControlFactory icf;
	Control* mbl = icf.createMouseButton( Input::M_LBTN );
	int mblIdx = m_manager->addControl(mbl);
	m_controlIdxs.push_back(mblIdx);
}

void InputSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_manager->update();
	double pressness = m_manager->getControl(m_controlIdxs[0])->getStatus();
	if(pressness == 1.0f)
		int korv=0;
}
