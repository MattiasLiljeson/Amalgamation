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
	AglParticleSystem particleSystem;
	string textureFileName;
};