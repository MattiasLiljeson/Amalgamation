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

		AglBlendMode_ALPHA,
		AglBlendMode_ADDITIVE,
		AglBlendMode_MULTIPLY,

		AglBlendMode_CNT
	};

	enum AglRasterizerMode {
		AglRasterizerMode_NON_EXISTING = -1,
		AglRasterizerMode_FIRST = 0,

		AglRasterizerMode_Z_CULLED = AglRasterizerMode_FIRST,
		AglRasterizerMode_ALWAYS_ON_TOP,

		AglRasterizerMode_LAST = AglRasterizerMode_ALWAYS_ON_TOP,
		AglRasterizerMode_CNT
	};

	enum AglSpace {
		AglSpace_NON_EXISTING = -1,

		AglSpace_GLOBAL,
		AglSpace_LOCAL,
		AglSpace_SCREEN,

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

	char				blendMode;		// used together with AglBlendMode
	char				rasterizerMode;	// used together with AglRasterizerMode
	char				spawnSpace;		// used together with AglSpace 
	char				particleSpace;	// used together with AglSpace 
	int					padding; // unused

	AglParticleSystemHeader()
	{
		alignmentType = AglParticleSystemHeader::OBSERVER;
		blendMode = AglBlendMode_ALPHA;
		color = AglVector4(1.0f, 1.0f, 1.0f, 1.0f);
		fadeInStop = 0;
		fadeOutStart = particleAge;
		maxOpacity = 1.0f;
		particleAge = 4.0f;
		particleFormat = AGL_PARTICLE_FORMAT_STANDARD;
		particleSize = AglVector2(1.0f, 1.0f);
		particleSpace = AglSpace_GLOBAL;
		particlesPerSpawn = 1;
		rasterizerMode = AglRasterizerMode_Z_CULLED;
		spawnAngularVelocity = 0;
		spawnDirection = AglVector3::left();
		spawnFrequency = 1.0f;
		spawnOffset = 0.0f;
		spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
		spawnPoint = AglVector3(0, 0, 0);
		spawnRotation = 0;
		spawnRotationOffset = 0;
		spawnSpace = AglSpace_LOCAL;
		spawnSpeed = 0;
		spawnType = AglParticleSystemHeader::CONTINUOUSLY;
		spread = 0;
		spreadType = AglParticleSystemHeader::INSPACE;
		textureNameIndex = -1;
	}
};