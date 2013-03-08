#pragma once
#include <Component.h>
#include <ComponentFactory.h>

class ShineSpawn: public Component
{
public:
	ShineSpawn(float p_createdAt, float p_lifetime) : Component(ComponentType::ShineSpawn)
	{
		m_createdAt = p_createdAt;
		m_lifetime = p_lifetime;
	}

public:
	float m_createdAt;
	float m_lifetime;
};