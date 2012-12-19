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

namespace TextureParser
{
	static void init();
	static ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, 
		const char* p_filePath);
	static ID3D11ShaderResourceView* createTexture( ID3D11Device* p_device,
		const byte* p_source, int p_width, int p_height, int p_pitch );

	///-----------------------------------------------------------------------------------
	/// Called once to initialize Free Image properly
	/// \return void
	///-----------------------------------------------------------------------------------
	static void init()
	{
		FreeImage_Initialise();
	}

	///-----------------------------------------------------------------------------------
	/// Handles the loading and creation of textures files. Supports various of types and
	/// throws exception if creation wasn't successfully.
	/// \param p_device
	/// \param p_filePath
	/// \return ID3D11ShaderResourceView*
	///-----------------------------------------------------------------------------------
	static ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, 
		const char* p_filePath)
	{
		FREE_IMAGE_FORMAT imageFormat;
		FIBITMAP* image;

		imageFormat = FreeImage_GetFIFFromFilename(p_filePath);
		if( imageFormat != FIF_UNKNOWN )
			image = FreeImage_Load(imageFormat, p_filePath);
		else
			throw FreeImageException("Unknown file format, cannot parse the file, " + 
			toString(p_filePath),__FILE__,__FUNCTION__,__LINE__);

		FreeImage_FlipVertical(image);

		ID3D11ShaderResourceView* newShaderResurceView = createTexture(
			p_device, FreeImage_GetBits(image), FreeImage_GetWidth(image),
			FreeImage_GetHeight(image), FreeImage_GetPitch(image));

		/************************************************************************/
		/* Clean up the mess afterwards											*/
		/************************************************************************/
		FreeImage_Unload(image);

		return newShaderResurceView;
	}

	static ID3D11ShaderResourceView* createTexture( ID3D11Device* p_device,
		const byte* p_source, int p_width, int p_height, int p_pitch )
	{
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = (void*)p_source;
		data.SysMemPitch = p_pitch;

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
		texDesc.Width				= p_width;
		texDesc.Height				= p_height;
		texDesc.MipLevels			= 1;
		texDesc.ArraySize			= 1;
		texDesc.SampleDesc.Count	= 1;
		texDesc.SampleDesc.Quality	= 0;
		texDesc.Usage				= D3D11_USAGE_DEFAULT;
		texDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags		= 0;
		texDesc.MiscFlags			= 0;

		ID3D11Texture2D* texture = NULL;
		HRESULT hr = p_device->CreateTexture2D( &texDesc, &data, &texture);

		if (FAILED(hr))
			throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
		ZeroMemory(&shaderDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderDesc.Format = texDesc.Format;
		shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderDesc.Texture2D.MipLevels = 1;
		shaderDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* newShaderResurceView;
		hr = p_device->CreateShaderResourceView( texture, &shaderDesc, 
			&newShaderResurceView);

		if (FAILED(hr))
			throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);

		texture->Release();

		return newShaderResurceView;
	}
}
