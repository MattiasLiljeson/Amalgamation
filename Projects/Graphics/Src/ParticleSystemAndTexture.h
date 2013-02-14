#pragma once

#include <AglParticleSystem.h>

struct ParticleSystemAndTexture
{
	AglParticleSystem particleSystem;
	int textureIdx; //Index in TextureManager
};