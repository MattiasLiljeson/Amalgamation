#include "InputManager.h"


InputManager::InputManager()
{
	m_mlFetcher = new MessageLoopFetcher();
	m_xifetcher = new XInputFetcher();
}

InputManager::~InputManager()
{
	delete m_mlFetcher;
	m_mlFetcher = NULL;
	delete m_xifetcher;
	m_xifetcher = NULL;

	for( unsigned int i; i<m_controls.size(); i++)
	{
		delete m_controls[i];
		m_controls[i] = NULL;
	}
}

int InputManager::addControl( Control* p_control )
{
	m_controls.push_back( p_control );
	return m_controls.size();
}
