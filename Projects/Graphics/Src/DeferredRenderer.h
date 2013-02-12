#pragma once

#include "Buffer.h"
#include "D3DException.h"
#include "PTVertex.h"
#include "RenderStateHelper.h"
#include <d3d11.h>

class BufferFactory;
class ShaderBase;
class DeferredBaseShader;
class DeferredComposeShader;
class LightShader;
class GUIShader;
class ShaderFactory;
class RenderStateHelper;

struct RasterizerFillMode;
struct RasterizerCullMode;
struct RasterizerFaceVertexOrder;
struct BlendState;
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
	/************************************************************************/
	/* See wiki for more details.											*/
	/* https://github.com/BiceMaster/PA2505-Stort-Spelprojekt-Kod/wiki/GBuffers */
	/************************************************************************/
	const static int BASESHADERS = 3;
	const static int RT0 = 0;
	const static int RT1 = 1;
	const static int RT2 = 2;
	const static int RT3 = 3;
	const static int RT4 = 4;
	enum RenderTargets{
		DIFFUSE, NORMAL, SPECULAR, LIGHT, DEPTH, NUMTARGETS,
	};
public:
	// ===================================================================================
	// Setup
	// ===================================================================================
	DeferredRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		int p_width, int p_height);
	virtual ~DeferredRenderer();

	///-----------------------------------------------------------------------------------
	/// Clear the buffers used by the deferred renderer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void clearBuffers();

	void setBasePassRenderTargets();


	///-----------------------------------------------------------------------------------
	/// Render a full screen quad textured with the gbuffer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void mapDeferredBaseRTSToShader(ID3D11ShaderResourceView* p_shadowMap);

	void mapDeferredBaseRTSToShader();

	void unmapDeferredBaseFromShader();

	void mapVariousPassesToComposeStage();
	
	void unmapVariousPassesFromComposeStage();

	void unmapDepthFromShaderVariables();
	// ===================================================================================
	// GUI Render
	// ===================================================================================

	// ===================================================================================
	// Blend states
	// ===================================================================================

	///-----------------------------------------------------------------------------------
	/// Set blend state to draw with
	/// \return void
	///-----------------------------------------------------------------------------------
	void setBlendState(BlendState::Mode p_state);
	
	void setBlendFactors(float p_red, float p_green, float p_blue, float p_alpha);

	void setBlendFactors(float p_oneValue);

	void setBlendMask(UINT p_mask);

	void setLightRenderTarget();

	BlendState::Mode getCurrentBlendStateType() {return m_currentBlendStateType;}

	///-----------------------------------------------------------------------------------
	/// Set settings for rasterizer states
	/// \return void
	///-----------------------------------------------------------------------------------
	void setRasterizerStateSettings(RasterizerState::Mode p_state);

	RasterizerState::Mode getCurrentRasterizerStateType() {return m_currentRasterizerStateType;}

	void releaseRenderTargetsAndDepthStencil();

	void initRendertargetsAndDepthStencil( int p_width, int p_height );

	ID3D11DepthStencilView* getDepthStencil();

	DeferredBaseShader* getDeferredBaseShader();

	LightShader* getDeferredLightShader();

	void renderSsao();

	void renderComposeStage();

	ID3D11ShaderResourceView*const* getShaderResourceView(RenderTargets p_target);

	// ===================================================================================
	// Debug
	// ===================================================================================
	void hookUpAntTweakBar();
private:
	void initDepthStencil();
	void initGeometryBuffers();
	void buildBlendStates();
	void buildRasterizerStates();
	void unMapGBuffers();
	void initShaders();
	void initFullScreenQuad();
	void initSSAO();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ShaderFactory*			m_shaderFactory;
	BufferFactory*			m_bufferFactory;

	ID3D11RenderTargetView*		m_gBuffers[NUMTARGETS];
	ID3D11ShaderResourceView*	m_gBuffersShaderResource[NUMTARGETS];
	ID3D11DepthStencilView*		m_depthStencilView;

	DeferredBaseShader*		m_baseShader;
	LightShader*			m_lightShader;
	DeferredComposeShader*	m_ssaoShader;
	DeferredComposeShader*	m_composeShader;

	Buffer<PTVertex>* m_fullscreenQuad;

	// blend states
	vector<ID3D11BlendState*> m_blendStates;
	BlendState::Mode m_currentBlendStateType;
	float m_blendFactors[4];
	UINT m_blendMask;

	// rasterizer states
	vector<ID3D11RasterizerState*> m_rasterizerStates;
	RasterizerState::Mode m_currentRasterizerStateType;

	SSAOBuffer	m_ssaoData;

	int m_width;
	int m_height;
};