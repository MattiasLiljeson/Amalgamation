#pragma once

#include <string>
#include <AglMatrix.h>

// =======================================================================================
//                                      InstanceInstruction
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Instance instruction for mesh loading
///        
/// # InstanceInstruction
/// Detailed description.....
/// Created on: 24-1-2013 
///---------------------------------------------------------------------------------------

struct InstanceInstruction
{
	std::string filename;
	AglMatrix transform;
};