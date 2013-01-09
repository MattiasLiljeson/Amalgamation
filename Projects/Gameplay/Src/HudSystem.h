#pragma once
#include <EntitySystem.h>

class LibRocketBackendSystem;

class HudSystem :	public EntitySystem
{
public:
	HudSystem( LibRocketBackendSystem* p_backend );
	~HudSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	LibRocketBackendSystem* m_backend;
};

