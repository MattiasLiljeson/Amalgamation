#pragma once
#include <AglVector3.h>
#include <AglMatrix.h>

// =======================================================================================
//                                      LightInstanceData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Data for an 
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

	float ambient[4];		// alpha not used
	float diffuse[4];		// alpha not used
	float specular[4];		// alpha is spec power

	int enabled;
	int type;				// Use E_LightTypes

	float pad[2];			// Padding

	LightInstanceData()
	{
		enabled = true;
		type = LightTypes::E_LightTypes_DIRECTIONAL;
		range = 0.0f;
		spotPower = 0.0f;
		
		//Identity
		for( int i=0; i<16; i++) {
			worldTransform[i] = 0.0f;
		}
		worldTransform[0] = worldTransform[5] = worldTransform[10] = worldTransform[15] = 1.0f;


		for( int i=0; i<3; i++) {
			lightDir[0] = 0.5773f; // 1/sqrt(3)
			attenuation[i] = 0.0f;
		}

		for( int i=0; i<4; i++) {
			ambient[i] = 0.0f;
			diffuse[i] = 0.0f;
			specular[i] = 0.0f;
		}

	}

	// Use setWorldTransform instead! Set range manually!
	//void setTranslation( AglVector3 p_translation )
	//{
	//	// worldTransform is col-major
	//	worldTransform[3] = p_translation.x;
	//	worldTransform[7] = p_translation.y;
	//	worldTransform[11] = p_translation.z;
	//}

	//void setScale( AglVector3 p_scale )
	//{
	//	worldTransform[0] = p_scale.x;
	//	worldTransform[5] = p_scale.y;
	//	worldTransform[10] = p_scale.z;
	//	worldTransform[15] = 1.0f;

	//	range = p_scale.length();
	//}

	void setWorldTransform( AglMatrix p_transform )
	{
		p_transform = AglMatrix::transpose( p_transform );
		for( int i=0; i<16; i++ ) {
			worldTransform[i] = p_transform[i];
		}
	}

};