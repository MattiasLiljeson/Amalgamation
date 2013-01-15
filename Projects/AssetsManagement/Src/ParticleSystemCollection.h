#pragma once

#include <vector>

using namespace std;

struct AglParticleSystem;

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
	vector<AglParticleSystem> m_collection;
};