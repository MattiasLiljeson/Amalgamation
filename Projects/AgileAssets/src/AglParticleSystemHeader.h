#pragma once
#include "AglVector3.h"
#include "Agl.h"

// =======================================================================================
//                                      AglParticleSystemHeader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglParticleSystemHeader
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

#define AGL_PARTICLE_FORMAT_STANDARD 0

struct AglParticleSystemHeader{
	enum AglSpawnType{
		ONCE, CONTINUOUSLY
	};
	enum AglSpawnOffsetType{
		ONSPHERE, INSPHERE, ONRING, INRING
	};
	enum AglSpreadType{
		INPLANE, INSPACE
	};
	enum AglAlignmentType{
		OBSERVER, SCREEN, WORLD, VELOCITY, 
	};

	enum AglBlendMode {
		AglBlendMode_NON_EXISTING = -1,

		AglBlendMode_ADDITIVE = 1,
		AglBlendMode_ALPHA = 2,
		AglBlendMode_MULTIPLY = 4,

		AglBlendMode_CNT = 3 // Update manually
	};

	enum AglRasterizerMode {
		AglRasterizerMode_NON_EXISTING = -1,
		AglRasterizerMode_FIRST = 8,

		AglRasterizerMode_ALWAYS_ON_TOP = AglRasterizerMode_FIRST,
		AglRasterizerMode_Z_CULLED = 16,

		AglRasterizerMode_LAST = AglRasterizerMode_Z_CULLED,
		AglRasterizerMode_CNT = 2 // Update manually
	};

	enum AglSpace {
		AglSpace_NON_EXISTING = -1,

		AglSpace_GLOBAL = 1,
		AglSpace_SPAWN_LOCAL = 2,
		AglSpace_PARTICLES_LOCAL = 4,

		AglSpace_CNT
	};

	AglVector3			spawnPoint;
	AglSpawnType		spawnType;
	float				spawnOffset;
	AglSpawnOffsetType	spawnOffsetType;
	AglVector3			spawnDirection;
	AglVector3			currSpawnDirection;
	float				spawnSpeed;
	float				spawnAngularVelocity;
	float				particleAge;
	float				spread; ///< 0 -> 1
	AglSpreadType		spreadType;
	float				spawnFrequency;
	AglParticleFormat	particleFormat;
	float				maxOpacity; ///< 0 -> 1
	unsigned int		particlesPerSpawn;
	AglVector2			particleSize;
	AglVector4			color;
	float				fadeInStop;
	float				fadeOutStart;
	int					textureNameIndex;
	AglAlignmentType	alignmentType;
	float				spawnRotation;
	float				spawnRotationOffset;
	int					modes; // used together with blend and rasterizer modes
	int					space; // used together with AglSpace 

	AglParticleSystemHeader()
	{
		particleAge = 4.0f;
		spawnPoint = AglVector3(0, 0, 0);
		particleFormat = AGL_PARTICLE_FORMAT_STANDARD;
		spawnDirection = AglVector3::left();
		spawnSpeed = 0;
		spawnAngularVelocity = 0;
		spread = 0;
		spawnFrequency = 1.0f;
		spawnOffset = 0.0f;
		spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
		spawnType = AglParticleSystemHeader::CONTINUOUSLY;
		maxOpacity = 1.0f;
		particlesPerSpawn = 1;
		spreadType = AglParticleSystemHeader::INSPACE;
		particleSize = AglVector2(1.0f, 1.0f);
		color = AglVector4(1.0f, 1.0f, 1.0f, 1.0f);
		fadeInStop = 0;
		fadeOutStart = particleAge;
		textureNameIndex = -1;
		alignmentType = AglParticleSystemHeader::OBSERVER;
		spawnRotation = 0;
		spawnRotationOffset = 0;

		modes = AglSpace_GLOBAL;
		space = AglSpace_SPAWN_LOCAL;
	}
};