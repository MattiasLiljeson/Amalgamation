#pragma once
#include <d3d11.h>
class ShaderFactory;

// =======================================================================================
//                                      ShadowMapRender
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShadowMapRender
/// Detailed description.....
/// Created on: 31-1-2013 
///---------------------------------------------------------------------------------------

class ShadowMapRenderer
{
public:
	ShadowMapRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		ShaderFactory* p_shaderFactory);
	~ShadowMapRenderer();
	void beginShadowPass();
private:
	void initDepthStencil();
public:
	const static int SHADOWMAPSIZE = 512;
private:
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext;
	ID3D11ShaderResourceView*	m_resourceView;
	ID3D11DepthStencilView*		m_depthStencilView;

	ShaderFactory*		m_shaderFactory;
};