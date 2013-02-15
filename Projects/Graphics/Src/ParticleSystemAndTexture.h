#pragma once

#include <AglParticleSystem.h>

struct ParticleSystemAndTexture
{
	AglParticleSystemHeader psOriginalSettings;
	AglParticleSystem particleSystem;
	int textureIdx; //Index in TextureManager
};