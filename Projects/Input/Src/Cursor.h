#pragma once

#include "Control.h"
#include "InputHelper.h"
#

class Cursor
{
public:
	Cursor(); // init all members to zero/NULL
	~Cursor();

	void setControls( Control* p_left, Control* p_right, Control* p_down, Control* p_up, 
		Control* p_primaryBtn, Control* p_secondaryBtn );

	void setSensitivity( double p_sensitivity );

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
	double m_sensitivity;
	Control* m_primaryBtn;
	Control* m_secondaryBtn;
	Control* m_xNegative;
	Control* m_xPositive;
	Control* m_yNegative;
	Control* m_yPositive;
};

