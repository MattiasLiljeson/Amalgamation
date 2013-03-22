#pragma once

#include <AglParticleSystem.h>

struct ParticleSystemAndTexture
{
	ParticleSystemAndTexture()
	{
		uvRect=AglVector4(0.0f,0.0f,1.0f,1.0f);
		originalSize = AglVector2(-1, -1);
	}

	AglParticleSystemHeader psOriginalSettings;
	AglParticleSystem particleSystem;
	int textureIdx; //Index in TextureManager
	AglVector4 uvRect; ///< cropping rect for particle
	AglVector2 originalSize;
};