#pragma once
#include <EntitySystem.h>
class SpeedFovAdjustSystem :
	public EntitySystem
{
public:
	SpeedFovAdjustSystem();
	virtual ~SpeedFovAdjustSystem();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:

};

