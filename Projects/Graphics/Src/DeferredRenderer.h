#pragma once

#include "Buffer.h"
#include "D3DException.h"
#include "PTVertex.h"
#include "RendererSceneInfo.h"
#include "ResourceManager.h"
#include <InstanceData.h>
#include <d3d11.h>

class BufferFactory;
class DeferredBaseShader;
class DeferredComposeShader;
class Mesh;
class RocketShader;
class ShaderFactory;
struct Texture;

/************************************************************************/
/* See wiki for more details.											*/
/* https://github.com/BiceMaster/PA2505-Stort-Spelprojekt-Kod/wiki/GBuffers */
/************************************************************************/
const static int NUMBUFFERS = 4;
const static int RT0 = 0;
const static int RT1 = 1;
const static int RT2 = 2;
const static int DEPTH = 3;

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

class DeferredRenderer
{
public:
	DeferredRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		int p_width, int p_height);
	virtual ~DeferredRenderer();

	///-----------------------------------------------------------------------------------
	/// Clear the buffers used by the deferred renderer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void clearBuffers();

	///-----------------------------------------------------------------------------------
	/// Sets the scene info, which can be regarded as "global" information to be used 
	/// when rendering. For example a world-view-projection matrix.
	/// \param p_sceneInfo
	/// \return void
	///-----------------------------------------------------------------------------------
	void setSceneInfo(const RendererSceneInfo& p_sceneInfo);

	///-----------------------------------------------------------------------------------
	/// Set the gbuffer as render target.
	/// \return void
	///-----------------------------------------------------------------------------------
	void beginDeferredBasePass();
	
	///-----------------------------------------------------------------------------------
	/// HACK: Temporary function to update the per frame CB in the middle of the frame
	/// \return void
	///-----------------------------------------------------------------------------------
	void updatePerFrameConstantBuffer();

	///-----------------------------------------------------------------------------------
	/// Render mesh data
	/// \param p_mesh
	/// \param p_texture
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderMesh(Mesh* p_mesh,
		Texture* p_texture);

	///-----------------------------------------------------------------------------------
	/// Render instanced mesh data
	/// \param p_mesh
	/// \param p_texture
	/// \param p_instanceBuffer
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderMeshInstanced(Mesh* p_mesh,
							 Texture* p_texture, 
							 Buffer<InstanceData>* p_instanceBuffer );

	///-----------------------------------------------------------------------------------
	/// Render a fullscreen quad textured with the gbuffer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderComposedImage();
	
	///-----------------------------------------------------------------------------------
	/// Desc
	/// \return void
	///-----------------------------------------------------------------------------------
	void beginRenderLibRocket();
	void renderLibRocket( Mesh* p_mesh, Texture* p_texture );
	void endRenderLibRocket();

	void hookUpAntTweakBar();

	void releaseRenderTargetsAndDepthStencil();
	void initRendertargetsAndDepthStencil( int p_width, int p_height );
private:
	void initDepthStencil();
	void initGeometryBuffers();
	void unMapGBuffers();
	void initTestShaders();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ShaderFactory*			m_shaderFactory;
	BufferFactory*			m_bufferFactory;

	RendererSceneInfo		m_sceneInfo;

	ID3D11RenderTargetView*		m_gBuffers[NUMBUFFERS];
	ID3D11ShaderResourceView*	m_gBuffersShaderResource[NUMBUFFERS];
	ID3D11DepthStencilView*		m_depthStencilView;

	DeferredBaseShader*		m_baseShader;
	DeferredComposeShader*	m_composeShader;
	RocketShader*			m_rocketShader;

	Buffer<PTVertex>* m_fullscreenQuad;

	// librocket stuph
	ID3D11BlendState* m_stdBlendState;
	UINT m_stdMask;

	int m_width;
	int m_height;
};