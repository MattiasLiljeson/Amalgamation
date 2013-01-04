#pragma once
#include <windows.h>
#include <FreeImage.h>
#include <d3d11.h>
#include "D3DException.h"
#include "FreeImageException.h"

// =======================================================================================
//                                      TextureParseer
// =======================================================================================
///---------------------------------------------------------------------------------------
/// \brief	Texture Parser wraps FreeImage and provides Texture parsing functions.
/// This namespace wraps the functionality behind FreeImage and provides a general 
/// data-to-DX11-SubResource function which can be used by others into one light weight
/// namespace. 
///        
/// # TextureParseer
/// Detailed description.....
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------

class TextureParser
{
public:
	enum TEXTURE_TYPE { RGBA, BGRA, RGB, BGR };

	static void init();
	static ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, 
		const char* p_filePath);
	static ID3D11ShaderResourceView* createTexture( ID3D11Device* p_device,
		const byte* p_source, int p_width, int p_height, int p_pitch,
		TEXTURE_TYPE p_type );
};
