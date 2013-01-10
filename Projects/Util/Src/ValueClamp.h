#pragma once
#include <algorithm>
using namespace std;

// =======================================================================================
//                                     Value clamping
// =======================================================================================


float clamp (float p_val, float p_min, float p_max)
{
	return max(p_min,min(p_max,p_val));
}


double clamp (double p_val, double p_min, double p_max)
{
	return max(p_min,min(p_max,p_val));
}


int clamp (int p_val, int p_min, int p_max)
{
	return max(p_min,min(p_max,p_val));
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
float saturate (float p_val)
{
	return clamp(p_val,0.0f,1.0f);
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
double saturate (double p_val)
{
	return clamp(p_val,0.0,1.0);
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
int saturate (int p_val)
{
	return clamp(p_val,0,1);
}

