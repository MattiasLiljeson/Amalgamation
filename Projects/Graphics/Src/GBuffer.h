// =======================================================================================
//                                      GBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains information need for a GBuffer
///        
/// # GBuffer
/// Detailed description.....
/// Created on: 29-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>

struct GBuffer
{
	ID3D11RenderTargetView* gBuffer;
	ID3D11ShaderResourceView* gBufferShaderResource;
};