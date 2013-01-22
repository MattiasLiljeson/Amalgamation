#include "RasterManager.h"

RasterManager* RasterManager::s_instance = 0;

RasterManager::RasterManager()
{
}
RasterManager::~RasterManager()
{
}

RasterManager* RasterManager::getInstance()
{
	if (!s_instance)
		s_instance = new RasterManager();
	return s_instance;
}
void RasterManager::cleanup()
{
	if (s_instance)
		delete s_instance;
	s_instance = 0;
}

void RasterManager::initialize(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	m_device->CreateRasterizerState(&rasterDesc, &m_wireframeState);

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	m_device->CreateRasterizerState(&rasterDesc, &m_standardState);

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	m_device->CreateRasterizerState(&rasterDesc, &m_invertedState);

	//Depth Stencil
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_device->CreateDepthStencilState(&depthStencilDesc, &mStandardDepthStencil);

	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	m_device->CreateDepthStencilState(&depthStencilDesc, &mNoCullDepthStencil);
}
void RasterManager::setStandardState()
{
	m_deviceContext->RSSetState(m_standardState);
}
void RasterManager::setWireframeState()
{
	m_deviceContext->RSSetState(m_wireframeState);
}
void RasterManager::setInvertedState()
{
	m_deviceContext->RSSetState(m_invertedState);
}
void RasterManager::dsSetStandard()
{
	m_deviceContext->OMSetDepthStencilState(mStandardDepthStencil, 1);
}
void RasterManager::dsSetNoCull()
{
	m_deviceContext->OMSetDepthStencilState(mNoCullDepthStencil, 1);
}