#pragma once

#include <AglParticleSystem.h>

struct ParticleSystemAndTexture
{
	ParticleSystemAndTexture(){uvRect=AglVector4(0.0f,0.0f,1.0f,1.0f);}
	AglParticleSystem particleSystem;
	int textureIdx; //Index in TextureManager
	AglVector4 uvRect; ///< cropping rect for particle
};