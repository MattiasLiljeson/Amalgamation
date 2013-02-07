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

struct SSAOBuffer
{
	float scale;
	float bias;
	float intensity;
	float sampleRadius;
	float epsilon;
	float empty[3];

	void setData(const SSAOBuffer& p_data){
		scale			= p_data.scale;
		bias			= p_data.bias;
		intensity		= p_data.intensity;
		sampleRadius	= p_data.sampleRadius;
		epsilon			= p_data.epsilon;
	}
};