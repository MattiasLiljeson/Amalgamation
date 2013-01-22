#pragma once

#include <Component.h>
#include <DebugUtil.h>

class ParticleSystemEmitter : public Component
{
public:
	ParticleSystemEmitter()
	{
	}

	~ParticleSystemEmitter() {}

	vector<unsigned int> particleSystems;
};