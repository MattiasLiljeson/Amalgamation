#pragma once
#include "AglVector3.h"
#include "AglVector2.h"

// =======================================================================================
//                                      AglStandardParticle
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglStandardParticle
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

struct AglStandardParticle
{
	AglVector3	position;
	float		age;
	AglVector3	velocity;
	float		angularVelocity;
	AglVector2	size;
	float		rotation;
	float		pad; // Padding: not used

	AglStandardParticle(AglVector3 p_position, AglVector3 p_velocity, AglVector2 p_size,
		float p_angularVelocity, float p_rotation);

	AglStandardParticle();
};