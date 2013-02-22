#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
#include <NumericBuffer.h>

struct AglVector3;

class SpeedFovAdjustSystem :
	public EntitySystem
{
public:
	SpeedFovAdjustSystem( float p_fov = 70.0f );
	virtual ~SpeedFovAdjustSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	static const float PI;
	float m_stdFov;
	float m_speedFovMult;
};

