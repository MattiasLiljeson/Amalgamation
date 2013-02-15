#pragma once
#include <AglVector4.h>
// =======================================================================================
//                                      MaterialInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains various textures ID 
///        
/// # MaterialInfo
/// Detailed description.....
/// Created on: 7-1-2013 
///---------------------------------------------------------------------------------------

struct MaterialInfo
{
	static const int SIZE = 6;
	static const unsigned int MAX_GRADIENTLAYERS = 4;
	unsigned int indexArray[SIZE];
	AglVector4 gradientLayers[MAX_GRADIENTLAYERS];
	float numberOfLayers;
	bool hasDisplacementMap;
	bool hasNormalMap;
	enum TextureTypes
	{
		DIFFUSEMAP, NORMALMAP, SPECULARMAP, GLOWMAP, DISPLACEMENTMAP, GRADIENTMAP, NUMTEXTURETYPES
	};

	MaterialInfo()
	{
		hasDisplacementMap=false;
		hasNormalMap=false;
		for (unsigned int i = 0; i<SIZE;i++){
			indexArray[i] = 0;
		}
		for (unsigned int i = 0; i <MAX_GRADIENTLAYERS; i++){
			gradientLayers[i] = AglVector4(1,1,1,1);
		}
		numberOfLayers = 1;
	}
	unsigned int getTextureType(TextureTypes p_textureType)
	{
		return indexArray[p_textureType];
	}
	void setTextureId(TextureTypes p_textureType, unsigned int p_textureId)
	{
		indexArray[p_textureType] = p_textureId;
	}
	void setGradientLayer(unsigned int p_index,const AglVector4& p_color){
		gradientLayers[p_index] = p_color;
	}
	void setNumberOfGradientLayers(float p_numberOfLayers){
		numberOfLayers = p_numberOfLayers;
	}
	AglVector4* getGradientColors(){
		return gradientLayers;
	}
	
};