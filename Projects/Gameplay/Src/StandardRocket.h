#pragma once
#include <Component.h>

class StandardRocket: public Component
{
public:
	float m_age;
	int m_ownerId;
	int m_target;
	StandardRocket();
	~StandardRocket();
private:

};