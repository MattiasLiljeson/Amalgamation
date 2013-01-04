#pragma once

#include "Control.h"
#include "InputHelper.h"
#include <vector>

using namespace std;

struct ControlSet
{
	Control* m_primaryBtn;
	Control* m_secondaryBtn;
	
	Control* m_xNegative;
	Control* m_xPositive;
	Control* m_yNegative;
	Control* m_yPositive;

	double m_xSensitivity;
	double m_ySensitivity;

	InputHelper::KEY_STATE getPrimaryState()
	{
		return getState( m_primaryBtn );
	}

	InputHelper::KEY_STATE getSecondaryState()
	{
		return getState( m_secondaryBtn );
	}

	InputHelper::KEY_STATE getState( Control* p_control )
	{
		int rawData = p_control->getRawData();
		return static_cast<InputHelper::KEY_STATE>(rawData); 
	}

	double dx()
	{
		double x = 0.0;
		x -= m_xNegative->getStatus() * m_xSensitivity;
		x += m_xPositive->getStatus() * m_xSensitivity;
		return x;
	}

	double dy()
	{
		double y = 0.0;
		y -= m_yNegative->getStatus() * m_ySensitivity;
		y += m_yPositive->getStatus() * m_ySensitivity;
		return y;
	}

};

class Cursor
{
public:
	Cursor(); // init all members to zero/NULL
	~Cursor();

	void addControlSet( double p_xSensitivity, double p_ySensitivity,
		Control* p_left, Control* p_right, Control* p_down, Control* p_up,
		Control* p_primaryBtn, Control* p_secondaryBtn );
	void addControlSet( ControlSet p_controlSet );

	void update();

	double getX();
	double getY();

	InputHelper::KEY_STATE getPrimaryState();
	double getPrimaryStatus();
	double getPrimaryDelta();

	InputHelper::KEY_STATE getSecondaryState();
	double getSecondaryStatus();
	double getSecondaryDelta();

private:
	double m_x; // double used internally in input
	double m_y; // double used internally in input
	vector<ControlSet> m_controlSets;
};

