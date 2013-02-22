#pragma once
#include <EntitySystem.h>
#include <NumericBuffer.h>

using namespace std;

class ShipParticleSystemUpdater : public EntitySystem
{
public:
	ShipParticleSystemUpdater();
	virtual ~ShipParticleSystemUpdater();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	NumericBuffer<float,10> m_speedBuffer;
};

