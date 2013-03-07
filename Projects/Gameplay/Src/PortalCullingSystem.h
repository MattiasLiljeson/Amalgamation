#pragma once

#include <EntitySystem.h>

class PortalCullingSystem : public EntitySystem
{
public:
	PortalCullingSystem();
	virtual ~PortalCullingSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );
};

