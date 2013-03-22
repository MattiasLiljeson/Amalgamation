#pragma once
#include <EntitySystem.h>
#include <NumericBuffer.h>

using namespace std;

struct ParticleSystemAndTexture;
struct AglParticleSystemHeader;

class ShipParticleSystemUpdater : public EntitySystem
{
public:
	ShipParticleSystemUpdater();
	virtual ~ShipParticleSystemUpdater();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

	void calculateThrustParticle(float p_thrust, 
		ParticleSystemAndTexture* p_psAndTex,
		AglParticleSystemHeader* p_header);
private:
	NumericBuffer<float,10> m_speedBuffer;
	bool m_maxiumSpeed;
	bool m_visible;
};

