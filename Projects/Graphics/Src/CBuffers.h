#pragma once
#include <AglMatrix.h>
// =======================================================================================
//                                      CBuffers
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Multiple Constant buffers
///        
/// # CBuffers
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------

struct SimpleCBuffer
{
	float color[4];
	float vp[16];
	float vpInv[16];

	void setViewProjection(const AglMatrix& p_vp){
		for(int i = 0 ; i < 16; i++){
			vp[i] = p_vp[i];
		}
	}

	void setViewProjectionInverse(const AglMatrix& p_vpInv){
		for(int i = 0 ; i < 16; i++){
			vpInv[i] = p_vpInv[i];
		}
	}
};