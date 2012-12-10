#ifndef AGLMATERIALMAPPING_H
#define AGLMATERIALMAPPING_H

// =================================================================================================
//                                         AglMaterialMapping
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Maps a material to a mesh.
///
/// 
/// # AglMaterialMapping
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglMaterialMapping
{
	unsigned int materialID;	///< The index of the material
	unsigned int meshID;		///< The index of the mesh
};

#endif