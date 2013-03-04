#pragma once

#include <AglParticleSystem.h>
#include <string>

using namespace std;

// =======================================================================================
//                                      ParticleSystemInstruction
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains instructions for creating a particlesystem
///        
/// # ParticleSystemInstruction
/// Detailed description.....
/// Created on: 7-2-2013 
///---------------------------------------------------------------------------------------

struct ParticleSystemInstruction
{
	ParticleSystemInstruction()
	{
		uvRect = AglVector4( 0.0f, 0.0f, 1.0f, 1.0f );
	}

	AglParticleSystem particleSystem;
	string textureFileName;
	AglVector4 uvRect; ///< cropping rect for particle
};