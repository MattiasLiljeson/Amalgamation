#pragma once

// =======================================================================================
//                                      ComposeCBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains all data needed by the CBuffer
///        
/// # ComposeCBuffer
/// Detailed description.....
/// Created on: 13-3-2013 
///---------------------------------------------------------------------------------------

struct ComposeCBuffer
{
	float circleOfConfusion;
	float empty[3];

	void setData(const ComposeCBuffer& p_data){
		circleOfConfusion = p_data.circleOfConfusion;
	}
};