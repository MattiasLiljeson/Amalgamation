#pragma once
#include <algorithm>
using namespace std;

// =======================================================================================
//                                     Value clamping
// =======================================================================================


static float clamp (float p_val, float p_min, float p_max)
{
	return max(p_min,min(p_max,p_val));
}


static double clamp (double p_val, double p_min, double p_max)
{
	return max(p_min,min(p_max,p_val));
}


static int clamp (int p_val, int p_min, int p_max)
{
	return max(p_min,min(p_max,p_val));
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
static float saturate (float p_val)
{
	return clamp(p_val,0.0f,1.0f);
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
static double saturate (double p_val)
{
	return clamp(p_val,0.0,1.0);
}

///-----------------------------------------------------------------------------------
/// Saturate a value to be between 0 and 1
/// \param val
/// \return float
///-----------------------------------------------------------------------------------
static int saturate (int p_val)
{
	return clamp(p_val,0,1);
}

