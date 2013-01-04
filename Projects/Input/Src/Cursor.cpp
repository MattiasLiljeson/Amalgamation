#include "Cursor.h"

// For NULL
#include <wchar.h>

Cursor::Cursor()
{
	//leftBtnState = InputHelper::KEY_STATE::KEY_UP;
	m_x = 0;
	m_y = 0;
	m_sensitivity = 25000.0;
	m_xPositive = NULL;
	m_xNegative = NULL;
	m_yPositive = NULL;
	m_yNegative = NULL;
}

Cursor::~Cursor()
{
}

void Cursor::setControls( Control* p_left, Control* p_right, Control* p_down, Control* p_up, 
						 Control* p_primaryBtn, Control* p_secondaryBtn )
{
	m_xNegative = p_left;
	m_xPositive = p_right;
	m_yNegative = p_down;
	m_yPositive = p_up;

	m_primaryBtn = p_primaryBtn;
	m_secondaryBtn = p_secondaryBtn;
}

void Cursor::setSensitivity( double p_sensitivity )
{
	m_sensitivity = p_sensitivity;
}

void Cursor::update()
{
	// raw data for the mouse buttons are enums of type KEY_STATE
	//leftBtnState = m_primaryBtn->getRawData();

	m_x -= m_xNegative->getStatus()*m_sensitivity;
	m_x += m_xPositive->getStatus()*m_sensitivity;
	m_y -= m_yNegative->getStatus()*m_sensitivity;
	m_y += m_yPositive->getStatus()*m_sensitivity;
}

double Cursor::getX()
{
	return m_x;
}

double Cursor::getY()
{
	return m_y;
}

InputHelper::KEY_STATE Cursor::getPrimaryState()
{
	return static_cast<InputHelper::KEY_STATE>(m_primaryBtn->getRawData()); 
}

double Cursor::getPrimaryStatus()
{
	return m_primaryBtn->getStatus();
}

double Cursor::getPrimaryDelta()
{
	return m_primaryBtn->getDelta();
}

InputHelper::KEY_STATE Cursor::getSecondaryState()
{
	return static_cast<InputHelper::KEY_STATE>(m_secondaryBtn->getRawData()); 
}

double Cursor::getSecondaryStatus()
{
	return m_secondaryBtn->getStatus();
}

double Cursor::getSecondaryDelta()
{
	return m_secondaryBtn->getDelta();
}
