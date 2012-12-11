#include "InputManager.h"


InputManager::InputManager( MessageLoopFetcher* p_mlFetcher, XInputFetcher* p_xiFetcher )
{
	m_mlFetcher = p_mlFetcher;
	m_xiFetcher = p_xiFetcher;
}

InputManager::~InputManager()
{
	delete m_mlFetcher;
	m_mlFetcher = NULL;
	delete m_xiFetcher;
	m_xiFetcher = NULL;

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

MessageLoopFetcher* InputManager::getMessageLoopFetcher() const
{
	return m_mlFetcher;
}

XInputFetcher* InputManager::getXInputFetcher() const
{
	return m_xiFetcher;
}

void InputManager::update()
{
	if( m_mlFetcher != NULL )
	{
		m_mlFetcher->update();
	}
	if( m_xiFetcher != NULL )
	{
		m_xiFetcher->update();
	}
	for( unsigned int i=0; i<m_controls.size(); i++)
		m_controls[i]->update( this );
}
