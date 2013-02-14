// =======================================================================================
//                                      SkeletonMappingVertex
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains a mapping to bones in a skeleton. Supports four bones per vertex
///        
/// # PNTTBVertex
/// Detailed description.....
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

struct SkeletonMappingVertex //IDENTICAL TO AglSkeletonMappingVertex
{
	float weight[8];
	unsigned int index[8];
};