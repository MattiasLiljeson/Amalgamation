// =======================================================================================
//                                      Texture
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wrapper for shader resource view. Wraps release on destroy, etc.
///        
/// # Texture
/// Detailed description.....
/// Created on: 6-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "D3DUtil.h"

struct Texture
{
	Texture(ID3D11ShaderResourceView* p_data)
	{
		data=p_data;
	}
	~Texture()
	{
		SAFE_RELEASE(data);
	}
	ID3D11ShaderResourceView* data;
};