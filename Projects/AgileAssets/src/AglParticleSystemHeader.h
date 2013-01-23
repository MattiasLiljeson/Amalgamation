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
	}
};