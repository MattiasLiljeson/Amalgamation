#pragma once
// =======================================================================================
// RandomUtil
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # RandomUtil
/// Detailed description...
/// Created on: 15-2-2013 
///---------------------------------------------------------------------------------------
class RandomUtil
{
public:
	///-----------------------------------------------------------------------------------
	/// Generate a random single precision value between 0.0 and 1.0.
	/// \return float
	///-----------------------------------------------------------------------------------
	static float randomSingle();
	///-----------------------------------------------------------------------------------
	/// Generate a random single precision value between p_min and p_max.
	/// \param p_min
	/// \param p_max
	/// \return float
	///-----------------------------------------------------------------------------------
	static float randomInterval(float p_min, float p_max);
	static int randomInteger(int p_count);
	static int randomIntegerInterval(int p_min, int p_max);
	static void randomEvenlyDistributedSphere(float* out_x, float* out_y, float* out_z);
};