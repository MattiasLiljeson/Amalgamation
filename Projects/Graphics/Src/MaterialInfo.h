#pragma once

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
	static const int SIZE = 4;
	unsigned int indexArray[SIZE];

	enum TextureTypes
	{
		DIFFUSEMAP, NORMALMAP, SPECULARMAP, GLOWMAP
	};

	MaterialInfo()
	{
		for (unsigned int i = 0; i<SIZE;i++)
		{
			indexArray[i] = 0;
		}
	}
	unsigned int getTextureType(TextureTypes p_textureType)
	{
		return indexArray[p_textureType];
	}
	void setTextureId(TextureTypes p_textureType, unsigned int p_textureId)
	{
		indexArray[p_textureType] = p_textureId;
	}
};