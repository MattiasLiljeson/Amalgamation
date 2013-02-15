#pragma once
#include <Component.h>

class StandardRocket: public Component
{
public:
	float m_age;
	int m_ownerId;
	StandardRocket();
	~StandardRocket();
private:

};