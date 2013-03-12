#pragma once
#include <AglMatrix.h>
#include <Globals.h>

// =======================================================================================
//                                      ShadowCBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShadowCBuffer
/// Detailed description.....
/// Created on: 31-1-2013 
///---------------------------------------------------------------------------------------

struct ShadowCBuffer
{
	float viewProj[16*MAXSHADOWS];

	void setViewProjection(const AglMatrix& p_viewProj){
		for (unsigned int i = 0; i < 16; i++){
			viewProj[i] = p_viewProj[i];
		}
	}
	void setViewProjections( AglMatrix* p_viewProjections){
		for (int i = 0; i < MAXSHADOWS; i++){
			for (int j = 0; j < 16; j++){
				viewProj[i*16+j] = p_viewProjections[i].data[j];
			}
		}
	}
};