#pragma once
#include <AglVector4.h>
// =======================================================================================
//                                      InstanceVertex
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Data for an instance. Transformation, colour, etc.     
/// # InstanceVertex
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

struct InstanceData
{
	float worldTransform[16];
	float gradientColors[16];
	float flags[4];			//Contains numberOfActiveGradient layers,

	void setWorldTransform(float* p_worldTransform){
		for (int i = 0 ; i < 16; i++){
			worldTransform[i] = p_worldTransform[i];
		}
	}
	void setGradientColor(AglVector4* p_gradientColors){
		int arrayIndex = 0;
		for (int i = 0 ; i < 16; i+=4){
			gradientColors[i] = p_gradientColors[arrayIndex].x;
			gradientColors[i+1] = p_gradientColors[arrayIndex].y;
			gradientColors[i+2] = p_gradientColors[arrayIndex].z;
			gradientColors[i+3] = p_gradientColors[arrayIndex].w;
			
			arrayIndex++;
		}
	}
	void setNumberOfActiveGradientLayers(float p_value){
		flags[0] = p_value;
	}
};