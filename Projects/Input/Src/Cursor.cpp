#include "Cursor.h"

#include "Control.h"
#include "ControlSet.h"

// For NULL
#include <wchar.h>

Cursor::Cursor()
{
	m_visible=true;
	reset();
}

Cursor::~Cursor()
{
}

void Cursor::addControlSet( double p_xSensitivity, double p_ySensitivity,bool p_useDeltaTime,
						   Control* p_left, Control* p_right, Control* p_down, Control* p_up,
						   Control* p_primaryBtn, Control* p_secondaryBtn )
{
	ControlSet set;
	set.m_useDeltaTime = p_useDeltaTime;
	set.m_xSensitivity = p_xSensitivity;
	set.m_ySensitivity = p_ySensitivity;
	set.m_xNegative = p_left;
	set.m_xPositive = p_right;
	set.m_yNegative = p_down;
	set.m_yPositive = p_up;
	set.m_primaryBtn = p_primaryBtn;
	set.m_secondaryBtn = p_secondaryBtn;
	addControlSet(set);

}

void Cursor::addControlSet( ControlSet p_controlSet )
{
	m_controlSets.push_back( p_controlSet );
}

void Cursor::update(float p_dt)
{
	// raw data for the mouse buttons are enums of type KeyStates
	//leftBtnState = m_primaryBtn->getRawData();

	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		double multiplier = 1.0f;
		if (m_controlSets[setIdx].m_useDeltaTime) multiplier=(double)p_dt;
		m_x += m_controlSets[setIdx].dx((double)m_screenWidth)*multiplier;
		m_y += m_controlSets[setIdx].dy((double)m_screenHeight)*multiplier;
	}

	// Keep cursor inside NDC
	if( m_x < -1.0)
	{
		m_x = -1.0;
	}
	else if( m_x > 1.0)
	{
		m_x = 1.0;
	}

	if( m_y < -1.0)
	{
		m_y = -1.0;
	}
	else if( m_y > 1.0)
	{
		m_y = 1.0;
	}
}

void Cursor::setScreenSize( int p_width, int p_height )
{
	m_screenWidth=p_width;
	m_screenHeight=p_height;
}

double Cursor::getX()
{
	return m_x;
}

double Cursor::getY()
{
	return m_y;
}

InputHelper::KeyStates Cursor::getPrimaryState() 
{
	//HACK: Return the first set that where the primary btn isn't up
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].getPrimaryState() != InputHelper::KeyStates_KEY_UP )
			return m_controlSets[setIdx].getPrimaryState(); 
	}

	return InputHelper::KeyStates_KEY_UP;
}

// HACK: A LOOOOOT of duplicated code below having some nice early returns. 

double Cursor::getPrimaryStatus()
{
	//HACK: Return the first set that where the input has moved
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].m_primaryBtn->getStatus() > 0.0f )
			return m_controlSets[setIdx].m_primaryBtn->getStatus(); 
	}

	return 0.0;
}

double Cursor::getPrimaryDelta()
{
	//HACK: Return the first set that where the input has moved
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].m_primaryBtn->getDelta() != 0.0f )
			return m_controlSets[setIdx].m_primaryBtn->getDelta(); 
	}

	return 0.0;
}

InputHelper::KeyStates Cursor::getSecondaryState()
{
	//HACK: Return the first set that where the primary btn isn't up
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].getSecondaryState() != InputHelper::KeyStates_KEY_UP )
			return m_controlSets[setIdx].getSecondaryState(); 
	}

	return InputHelper::KeyStates_KEY_UP;
}

double Cursor::getSecondaryStatus()
{
	//HACK: Return the first set that where the input has moved
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].m_secondaryBtn->getStatus() > 0.0f )
			return m_controlSets[setIdx].m_secondaryBtn->getStatus(); 
	}

	return 0.0;
}

double Cursor::getSecondaryDelta()
{
	//HACK: Return the first set that where the input has moved
	for( unsigned int setIdx=0; setIdx<m_controlSets.size(); setIdx++ )
	{
		if( m_controlSets[setIdx].m_secondaryBtn->getDelta() != 0.0f )
			return m_controlSets[setIdx].m_secondaryBtn->getDelta(); 
	}

	return 0.0;
}

void Cursor::reset()
{
	m_x = 0.0;
	m_y = 0.0;
}

void Cursor::overridePosition( float p_ndcX, float p_ndcY )
{
	m_x = p_ndcX;
	m_y = p_ndcY;
}
