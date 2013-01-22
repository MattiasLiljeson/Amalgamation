#pragma once

// =======================================================================================
//                                      LightInstanceData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Data for an instance. Transformation, colour, etc.     
/// # InstanceVertex
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

struct LightTypes
{
	enum E_LightTypes
	{
		E_LightTypes_DIRECTIONAL,
		E_LightTypes_POINT,
		E_LightTypes_SPOT,
	};
};

struct LightInstanceData
{
	float worldTransform[16];

	float range;
	float lightDir[3];

	float attenuation[3];	// Used in the formula: x, x^2, x^3
	float spotPower;		// Spot cone width. Power of 10.

	float ambient[4];
	float diffuse[4];
	float specular[4];

	int enabled;
	int type;				// Use E_LightTypes

	float pad[2];			// Padding
};