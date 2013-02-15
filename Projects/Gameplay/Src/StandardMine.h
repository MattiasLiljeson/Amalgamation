#pragma once
#include <Component.h>

class StandardMine: public Component
{
public:

	StandardMine();
	~StandardMine();

public:
	float m_age;
	float range;
	int m_ownerId;
};