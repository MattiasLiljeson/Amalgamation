#pragma once
#include <string>

// =======================================================================================
//                                      GPUTimerProfile
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # GPUTimerProfile
/// Detailed description.....
/// Created on: 24-2-2013 
///---------------------------------------------------------------------------------------

struct GPUTimerProfile
{
	std::string profile;
	double renderingTime;

	GPUTimerProfile(std::string p_profile){
		profile = p_profile;
		renderingTime = 0;
	}
};