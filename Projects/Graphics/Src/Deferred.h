// =======================================================================================
//                                      Deferred
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Deferred Pipeline
///        
/// # Deferred
/// The Geometry buffers (gBuffers) order is, depth, diffuse, normal.
/// Created on: 29-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "D3DException.h"
#include "Buffer.h"
#include "Shader.h"
#include "PTVertex.h"

const static int NUMBUFFERS = 3;
const static int DEPTH = 0;
const static int DIFFUSE = 1;
const static int NORMAL = 2;

class Deferred
{
public:
	Deferred(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, int p_width, 
		int p_height);
	virtual ~Deferred();
	void clearBuffers();
	void deferredBasePass();
	void renderComposedImage();
protected:
private:
	void initDepthStencil();
	void initGeomtryBuffers();
	void createFullScreenQuad();
	void unampGBuffers();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ID3D11RenderTargetView*		m_gBuffers[NUMBUFFERS];
	ID3D11RenderTargetView*		m_backBuffer;
	ID3D11ShaderResourceView*	m_gBuffersShaderResource[NUMBUFFERS];
	ID3D11DepthStencilView*		m_depthStencilView;

	Shader* m_shader;
	Buffer* m_vertexBuffer;

	int m_width;
	int m_height;
};