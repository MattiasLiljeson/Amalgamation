#pragma once
#include <EntitySystem.h>

class HudSystem :	public EntitySystem
{
public:
	HudSystem();
	~HudSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
};

