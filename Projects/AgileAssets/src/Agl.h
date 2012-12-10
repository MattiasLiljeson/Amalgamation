#ifndef AGL_H
#define AGL_H

#include <iostream>
#include "AglMatrix.h"
#include "AglVector2.h"
#include "AglVector3.h"
#include "AglVector4.h"
#include "AglMaterial.h"
#include "AglGradientMaterial.h"
#include "AglNode.h"
#include "AglMaterialMapping.h"

using namespace std;

// =================================================================================================
//                                         AglTopHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief TopHeader that starts every AGL file
///
/// 
/// # AglTopHeader
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglTopHeader
{
	int		nameID;					///< Index to the name of the file
	float	version;				///< Version of format
	int		nameArrayLength;		///< Length of name array
	int		meshCount;				///< Number of meshes in the file
	int		materialCount;			///< Number of materials in the file
	int		materialMappingCount;	///< Number of material mappings in the file
	int		nodeCount;				///< Number of nodes in the file
	int		skeletonCount;			///< Number of skeletons in the file
	int		skeletonMappingCount;	///< Number of skeleton mappings in the file
	int		nodeAnimationCount;		///< Number of node animations in the file
	int		animationLayerCount;	///< Number of animation layers in the file
	int		animationCount;			///< Number of animations in the file
	int		looseBspCount;			///< Number of loose bsp trees in the file
	int		SphereGridCount;		///< Number of sphere grids in the file
};

typedef int AglVertexFormat;

const float AGLVERSION = 0.1f;

#endif
