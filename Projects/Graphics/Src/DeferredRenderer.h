#pragma once

#include "Buffer.h"
#include "D3DException.h"
#include "PTVertex.h"
#include "RenderStateHelper.h"
#include <d3d11.h>

class BufferFactory;
class ShaderBase;
class DeferredBaseShader;
class DeferredAnimatedBaseShader;
class DeferredTessBaseShader;
class DeferredTessAnimatedBaseShader;
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
	const static int DEPTH_IDX = 10;
	//const static int RT5 = 5;
	enum RenderTargets {
		RenderTargets_NON_EXISTING	= -1,
		RenderTargets_DIFFUSE		= RT0,
		RenderTargets_NORMAL		= RT1,
		RenderTargets_SPECULAR		= RT2,
		RenderTargets_LIGHT_DIFFUSE = RT3,
		RenderTargets_LIGHT_SPEC	= RT4,
		//RenderTargets_DEPTH			= RT5,
		RenderTargets_CNT,
	};

public:
	DeferredRenderer( ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		int p_width, int p_height, bool p_enableHdr, bool p_enableEffects );
	virtual ~DeferredRenderer();
	// Setup
	void initRendertargetsAndDepthStencil( int p_width, int p_height );
	void releaseRenderTargetsAndDepthStencil();

	// Buffers as RTs
	void setBasePassRenderTargets();
	void setLightRenderTargets();
	void generateEffects();
	void setDofRenderTargets();
	void generateLowRes();
	void renderComposeStage();

	void setViewPortSize( float p_width, float p_height );

	// Buffers as resources
	void mapNormal(ID3D11ShaderResourceView* p_shadowMap);
	void mapNormal();
	void unmapNormal();
	void mapGbuffers();
	void unmapGbuffers();
	void mapDofBuffers();
	void unmapDofBuffers();
	void mapDepth();
	void unmapDepth();

	// Buffer / RT manipulation
	void clearRenderTargets();
	ID3D11DepthStencilView* getDepthStencil();
	ID3D11ShaderResourceView*const* getShaderResourceView(RenderTargets p_target);

	// Blend states
	void setBlendState(BlendState::Mode p_state);
	void setBlendFactors(float p_red, float p_green, float p_blue, float p_alpha);
	void setBlendFactors(float p_oneValue);
	void setBlendMask(UINT p_mask);
	BlendState::Mode getCurrentBlendStateType();

	// Rasterizer states
	void setRasterizerStateSettings(RasterizerState::Mode p_state);
	RasterizerState::Mode getCurrentRasterizerStateType();

	// Sampler states
	void setSamplerStates();

	// Shader getters
	DeferredBaseShader* getDeferredBaseShader();
	DeferredAnimatedBaseShader* getDeferredAnimatedBaseShader();
	DeferredTessBaseShader* getDeferredTessBaseShader();
	DeferredTessAnimatedBaseShader* getDeferredTessAnimatedBaseShader();
	LightShader* getDeferredLightShader();

	// Debug
	void hookUpAntTweakBar();

private:
	void initDepthStencil();
	void initGeometryBuffers();
	void initLightBuffers();
	void initDofBuffers();
	void buildBlendStates();
	void buildRasterizerStates();
	void initShaders();
	void initFullScreenQuad();
	void initSSAO();

	void unmapAllBuffers();

	void checkHr( HRESULT p_hr, const string& p_file,
		const string& p_function, int p_line );

	void createSrvAndRtv( ID3D11ShaderResourceView** out_srv,
		ID3D11RenderTargetView** out_rtv, int p_width, int p_height, DXGI_FORMAT p_format );

private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ShaderFactory*			m_shaderFactory;
	BufferFactory*			m_bufferFactory;

	ID3D11ShaderResourceView*	m_srvDepth;

	// Regular Gbuffers
	ID3D11RenderTargetView*		m_rtvGBuffers[RenderTargets_CNT];
	ID3D11ShaderResourceView*	m_srvGBuffers[RenderTargets_CNT];

	// Dof
	ID3D11RenderTargetView*		m_rtvDofBuffers[RenderTargets_CNT];
	ID3D11ShaderResourceView*	m_srvDofBuffers[RenderTargets_CNT];

	ID3D11DepthStencilView*		m_depthStencilView;

	DeferredBaseShader*				m_baseShader;
	DeferredAnimatedBaseShader*		m_animatedBaseShader;
	DeferredTessBaseShader*			m_tessBaseShader;
	DeferredTessAnimatedBaseShader*	m_tessAnimatedBaseShader;

	LightShader*			m_lightShader;
	DeferredComposeShader*	m_effectShader;
	DeferredComposeShader*	m_lowResGenerationShader;
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

	// Sampler States
	ID3D11SamplerState* m_samplerStates[SamplerState::SamplerState_CNT];

	SSAOBuffer	m_ssaoData;

	int m_width;
	int m_height;
	bool m_enableHdr;
	bool m_enableEffects;
};