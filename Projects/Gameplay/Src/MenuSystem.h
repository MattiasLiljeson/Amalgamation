#pragma once
#include <EntitySystem.h>
class MenuSystem : public EntitySystem
{
public:
	MenuSystem();
	~MenuSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
};

