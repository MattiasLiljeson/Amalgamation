#pragma once

#include <vector>
#include "ParticleSystemInstruction.h"

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

struct ParticleSystemInstructionCollection
{
	vector<ParticleSystemInstruction> m_collection;
};