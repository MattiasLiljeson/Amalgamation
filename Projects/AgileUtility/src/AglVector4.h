#ifndef AGLVECTOR4_H
#define AGLVECTOR4_H

#include <cmath>

//INCOMPLETE!!!!!!

// =================================================================================================
//                                         AglVector4
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a Vector4 with appropriate operations and functions.
///
/// 
/// # AglVector4
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglVector4
{
	float data[4];

	//Constructors
	AglVector4(float x, float y, float z, float w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}
	AglVector4(AglVector3 vec, float w)
	{
		data[0] = vec[0];
		data[1] = vec[1];
		data[2] = vec[2];
		data[3] = w;
	}
	AglVector4()
	{
		data[0] = data[1] = data[2] = data[3] = 0;
	}

	//Destructor
	~AglVector4()
	{
	}
};

#endif