// =======================================================================================
//                                      DeferredRenderer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Deferred Pipeline
///        
/// # DeferredRenderer
/// The Geometry buffers (gBuffers) order is, depth, diffuse, normal.
/// Created on: 29-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "AntTweakBarWrapper.h"
#include "D3DException.h"
#include "Buffer.h"
#include "ShaderFactory.h"
#include "BufferFactory.h"
#include "PTVertex.h"
#include "RendererSceneInfo.h"
#include "ResourceManager.h"
#include "Texture.h"

const static int NUMBUFFERS = 3;
const static int DEPTH = 2;
const static int NORMAL = 1;
const static int DIFFUSE = 0;

class DeferredRenderer
{
public:
	DeferredRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		int p_width, int p_height);
	virtual ~DeferredRenderer();

	///-----------------------------------------------------------------------------------
	/// Clear the buffers used by the deferred renderer.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void clearBuffers();

	///-----------------------------------------------------------------------------------
	/// Sets the scene info, which can be regarded as "global" information to be used 
	/// when rendering. For example a world-view-projection matrix.
	/// \param p_sceneInfo
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setSceneInfo(const RendererSceneInfo& p_sceneInfo);

	///-----------------------------------------------------------------------------------
	/// Set the gbuffer as render target.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void beginDeferredBasePass();

	///-----------------------------------------------------------------------------------
	/// Render mesh data
	/// \param p_meshInfo
	/// \returns void
	///-----------------------------------------------------------------------------------
	void renderMesh(Mesh* p_mesh,
					Texture* p_texture );

	///-----------------------------------------------------------------------------------
	/// Render a fullscreen quad textured with the gbuffer.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void renderComposedImage();
	void hookUpAntTweakBar();
protected:
private:
	void initDepthStencil();
	void initGeometryBuffers();
	void unMapGBuffers();
	void initTestShaders();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ShaderFactory*			m_shaderFactory;

	RendererSceneInfo		m_sceneInfo;

	ID3D11RenderTargetView*		m_gBuffers[NUMBUFFERS];
	ID3D11ShaderResourceView*	m_gBuffersShaderResource[NUMBUFFERS];
	ID3D11DepthStencilView*		m_depthStencilView;

	DeferredBaseShader*		m_baseShader;
	DeferredComposeShader*	m_composeShader;

	Buffer<PTVertex>* m_fullscreenQuad;

	int m_width;
	int m_height;
};