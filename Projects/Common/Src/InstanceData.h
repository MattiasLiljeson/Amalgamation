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
		float temp[16];
		for (int i = 0 ; i < 16; i+=4){
			temp[i] = p_gradientColors[arrayIndex].x;
			temp[i+1] = p_gradientColors[arrayIndex].y;
			temp[i+2] = p_gradientColors[arrayIndex].z;
			temp[i+3] = p_gradientColors[arrayIndex].w;
			
			arrayIndex++;
		}

		for (int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				gradientColors[i*4+j]=temp[j*4+i];
			}
		}
		
	}
	void setNumberOfActiveGradientLayers(float p_value){
		flags[0] = p_value;
	}
};