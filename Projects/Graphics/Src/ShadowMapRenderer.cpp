#include "ShadowMapRenderer.h"
#include "D3DException.h"
#include "D3DUtil.h"
#include "ShaderFactory.h"

ShadowMapRenderer::ShadowMapRenderer(ID3D11Device* p_device, 
									 ID3D11DeviceContext* p_deviceContext, 
									 ShaderFactory* p_shaderFactory)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_shaderFactory = p_shaderFactory;

	initDepthStencil();
}

ShadowMapRenderer::~ShadowMapRenderer()
{
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_resourceView);
}

void ShadowMapRenderer::initDepthStencil()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* shadowMapTexture;
	D3D11_TEXTURE2D_DESC descDepthTexture;
	descDepthTexture.Width = SHADOWMAPSIZE;
	descDepthTexture.Height = SHADOWMAPSIZE;
	descDepthTexture.MipLevels = 1;
	descDepthTexture.ArraySize = 1;
	descDepthTexture.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepthTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepthTexture.SampleDesc.Count = 1;
	descDepthTexture.SampleDesc.Quality = 0;
	descDepthTexture.CPUAccessFlags = 0;
	descDepthTexture.MiscFlags = 0;

	if (FAILED(hr = m_device->CreateTexture2D(&descDepthTexture,NULL,&shadowMapTexture)))
		throw D3DException(hr, __FILE__, __FUNCTION__, __LINE__);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;


	if (FAILED(hr=m_device->CreateDepthStencilView(
		shadowMapTexture,
		&depthStencilViewDesc,
		&m_depthStencilView)
		))
		throw D3DException(hr, __FILE__, __FUNCTION__, __LINE__);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	ZeroMemory(&shaderResourceDesc,sizeof(shaderResourceDesc));
	shaderResourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceDesc.Texture2D.MipLevels = 1;

	if (FAILED(hr=m_device->CreateShaderResourceView(
		shadowMapTexture,
		&shaderResourceDesc,
		&m_resourceView)
		))
		throw D3DException(hr, __FILE__, __FUNCTION__, __LINE__);

	shadowMapTexture->Release();
}

void ShadowMapRenderer::beginShadowPass()
{
	ID3D11RenderTargetView* temp = NULL;
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_deviceContext->OMSetRenderTargets(1, &temp, m_depthStencilView);
}
