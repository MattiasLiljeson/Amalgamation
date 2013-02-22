#pragma once
#include <EntitySystem.h>
class SpeedBufferUpdaterSystem : public EntitySystem
{
public:
	SpeedBufferUpdaterSystem();
	virtual ~SpeedBufferUpdaterSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
};

