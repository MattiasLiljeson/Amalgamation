#pragma once

#include <vector>
#include "AglParticleSystem.h"

using namespace std;



// =======================================================================================
//                              ParticleSystemCollection
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Collection of particle systems
///        
/// # HardPointCollection
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

struct ParticleSystemCollection
{
	vector<AglParticleSystem> m_particleSystems;
};