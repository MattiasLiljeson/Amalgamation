#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>

struct AglVector3;

class ValBuffer
{
public:
	static const int VAL_CNT = 25;
	ValBuffer( float p_initVal = 0.0f );
	void setAllVals( float p_val );
	float getAvg(); ///< Works well over the network. Long falloff
	float getLog(); ///< Small falloff. Works bad over the network.
	void addVal( float p_val );

private:
	float m_vals[VAL_CNT];
	int m_currIdx;
};



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
	AglVector3 m_oldPos;
	ValBuffer m_buffer;
	float m_speedFovMult;
};

