#pragma once
#include <EntitySystem.h>
class DamageUpdaterSystem : public EntitySystem
{
public:
	DamageUpdaterSystem();
	virtual ~DamageUpdaterSystem();
	virtual void processEntities( const vector<Entity*>& p_entities );
};

