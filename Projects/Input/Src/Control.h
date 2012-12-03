#pragma once
class Control
{
public:
	Control();
	virtual ~Control();

	///@brief Override and update "pressness" member  variables.
	virtual void update() = 0;

	///@brief
	///@return The current status of the control. Between 0.0 and 1.0
	virtual float getStatus();

	///@brief
	///@return The delta of status from last update and the update before. 
	virtual float getDelta();

protected:
	// Current amount of "Pressness" on the control 
	float m_status;

	// "Pressness" delta between latest update and the update before.
	float m_statusDelta;
};

