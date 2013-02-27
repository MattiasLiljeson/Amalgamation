#pragma once

#include <EntitySystem.h>

class InterpolationSystem2: public EntitySystem
{
public:
	InterpolationSystem2();

	~InterpolationSystem2();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();
};