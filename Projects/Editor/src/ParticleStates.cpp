#include "ParticleStates.h"

ID3D11BlendState* ParticleStates::AlphaBlending = NULL;
ID3D11BlendState* ParticleStates::AdditiveBlending = NULL;
ID3D11BlendState* ParticleStates::MultiplyBlending = NULL;
ID3D11DepthStencilState* ParticleStates::CullZ = NULL;
ID3D11DepthStencilState* ParticleStates::CullNone = NULL;

void ParticleStates::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	//CREATE ALL BLEND STATES

	//Alpha blending
	D3D11_BLEND_DESC blendStateDesc; 
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = FALSE;
	blendStateDesc.IndependentBlendEnable = FALSE;        
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pDevice->CreateBlendState(&blendStateDesc, &AlphaBlending);

	//Additive Blending
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	pDevice->CreateBlendState(&blendStateDesc, &AdditiveBlending);

	//Multiply Blending
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	pDevice->CreateBlendState(&blendStateDesc, &MultiplyBlending);


	//RASTERIZER STATES

	//Cull Z
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &CullZ);

	//Cull None
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &CullNone);
}