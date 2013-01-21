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

	AglVector3			spawnPoint;
	AglSpawnType		spawnType;
	float				spawnOffset;
	AglSpawnOffsetType	spawnOffsetType;
	AglVector3			spawnDirection;
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
};