#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
#include <NumericBuffer.h>

struct AglVector3;

class SpeedFovAdjustSystem :
	public EntitySystem
{
public:
	SpeedFovAdjustSystem();
	virtual ~SpeedFovAdjustSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	float m_fieldOfViewFactor;
	float m_offest;
	float m_maxAdjustment;
};

