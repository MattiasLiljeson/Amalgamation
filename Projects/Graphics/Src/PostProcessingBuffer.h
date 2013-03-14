#pragma once

// =======================================================================================
//                                      SSAOCBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # SSAOCBuffer
/// Detailed description.....
/// Created on: 7-2-2013 
///---------------------------------------------------------------------------------------

struct PostProcessingBuffer
{
	float scale;
	float bias;
	float intensity;
	float sampleRadius;
	
	float stopNear;
	float stopFar;
	float startNear;
	float startFar;

	void setData(const PostProcessingBuffer& p_data){
		scale			= p_data.scale;
		bias			= p_data.bias;
		intensity		= p_data.intensity;
		sampleRadius	= p_data.sampleRadius;
		stopNear		= p_data.stopNear;
		stopFar			= p_data.stopFar;
		startNear		= p_data.startNear;
		startFar		= p_data.startFar;
	}
};