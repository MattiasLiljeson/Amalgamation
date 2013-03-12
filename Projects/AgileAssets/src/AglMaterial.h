#ifndef AGLMATERIAL_H
#define AGLMATERIAL_H

#include "AglVector3.h"
#include "AglVector4.h"

// =================================================================================================
//                                         AglMaterial
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies an Agl Phong based material.
///
/// 
/// # AglMaterial
/// The Material supports necessary data for phong shading. Both data using textures
/// and implicitly stated variables can be used.
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglMaterial
{
	int id;								///< ID of the material
	int nameID;							///< Index to the name in the name array
	AglVector3 ambient;					///< Ambient color of the material
	AglVector3 diffuse;					///< Diffuse color of the material. Inactive when texture is used
	AglVector3 specular;				///< Specular color of the material. Inactive when texture is used
	AglVector3 emissive;				///< Emissive color of the material. Inactive when glow texture is used
	float opacity;						///< Opacity of the material [0, 1]
	float shininess;					///< Shininess of the material [1, +inf]
	float reflectivity;					///< Reflectivity of the material [0, 1]
	float displacement;					///< Displacement mapping displacement factor of the material [-inf, inf]
	AglVector4 tesselationFactor;		///< Tesselation factors of the material around edge 1, 2, 3 and interior respectively.
	int diffuseTextureNameIndex;		///< Index to diffuse texture. -1 IF texture is absent
	int specularTextureNameIndex;		///< Index to specular texture. -1 IF texture is absent
	int normalTextureNameIndex;			///< Index to normal texture. -1 IF texture is absent
	int glowTextureNameIndex;			///< Index to glow texture. -1 IF texture is absent
	int displacementTextureNameIndex;	///< Index to displacement texture. -1 IF texture is absent
	int gradientTextureNameIndex;		///< Index to gradient texture
	int gradientDataIndex;				///< Index to the color data used by the gradient texture
	float textureScale;					///< Scale of the textures. [0, +inf]

	AglMaterial();
};

#endif