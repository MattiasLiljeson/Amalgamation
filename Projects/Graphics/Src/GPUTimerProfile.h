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
	std::vector<double> renderingTimes;
	std::vector<double> averageRenderingTimes;
	double renderingTime;
	double renderingSpike;
	double renderingAverage;

	GPUTimerProfile(std::string p_profile){
		profile = p_profile;
		renderingTime = 0;
		renderingSpike = 0;
		renderingAverage = 0;
	}
	void pushNewTime(double p_newRenderingTime){
		renderingTimes.push_back(p_newRenderingTime);
		renderingTime = p_newRenderingTime;
		if(renderingTime > renderingSpike){
			renderingSpike = renderingTime;
		}
	}
	void calculateAvarage(){
		renderingAverage = 0;
		for(unsigned int i = 0; i < renderingTimes.size(); i++){
			renderingAverage += renderingTimes[i];
		}
		renderingAverage = renderingAverage / static_cast<double>(renderingTimes.size());
		averageRenderingTimes.push_back(renderingAverage);
		renderingTimes.clear();
	}
};