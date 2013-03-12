#include "InputManager.h"
#include "Control.h"
#include "DirectInputFetcher.h"
#include "IMouseKeyboardFetcher.h"
#include "MessageLoopFetcher.h"
#include "XInputFetcher.h"


InputManager::InputManager( IMouseKeyboardFetcher* p_mlFetcher, XInputFetcher* p_xiFetcher )
{
	m_mouseKeyboardFetcher = p_mlFetcher;
	m_xInputFetcher = p_xiFetcher;
	double joyPadDeadZone = 0.3;

	m_xInputFetcher->calibrate( joyPadDeadZone );
}

InputManager::~InputManager()
{
	delete m_mouseKeyboardFetcher;
	m_mouseKeyboardFetcher = NULL;
	delete m_xInputFetcher;
	m_xInputFetcher = NULL;

	for( unsigned int i=0; i<m_controls.size(); i++)
	{
		delete m_controls[i];
		m_controls[i] = NULL;
	}
}

int InputManager::addControl( Control* p_control )
{
	m_controls.push_back( p_control );
	return m_controls.size() -1; //size - 1 = index of the added control
}

Control* InputManager::getControl( int p_idx )
{
	if( 0 <= p_idx  && p_idx < (int)m_controls.size())
		return m_controls[p_idx];
	else
		return NULL;
}

IMouseKeyboardFetcher* InputManager::getMouseKeyboardFetcher() const
{
	return m_mouseKeyboardFetcher;
}

XInputFetcher* InputManager::getXInputFetcher() const
{
	return m_xInputFetcher;
}

void InputManager::update()
{
	if( m_mouseKeyboardFetcher != NULL )
	{
		m_mouseKeyboardFetcher->update();
	}
	if( m_xInputFetcher != NULL )
	{
		m_xInputFetcher->update();
	}
	for( unsigned int i=0; i<m_controls.size(); i++)
		m_controls[i]->update( this );
}
