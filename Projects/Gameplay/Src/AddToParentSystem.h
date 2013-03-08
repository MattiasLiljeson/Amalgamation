#pragma once

#include <EntitySystem.h>
#include <AglBoundingSphere.h>

class AddToParentSystem : public EntitySystem
{
private:
	AglBoundingSphere getBoundingSphereWorld(Entity* p_entity);
public:
	AddToParentSystem();
	virtual ~AddToParentSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

private:
};

