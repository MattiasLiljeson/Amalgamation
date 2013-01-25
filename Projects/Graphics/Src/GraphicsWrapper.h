// =======================================================================================
//                                      GraphicsWrapper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A wrapper for Direct3D functionality, handles objects for creating and
/// rendering graphics data.
///        
/// # GraphicsWrapper
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <ResourceManager.h>
#include <Globals.h>
#include "Buffer.h"
#include "BufferFactory.h"
#include "DeferredRenderer.h"
#include "RendererSceneInfo.h"
#include "InstanceData.h"
#include "TextureParser.h"
#include "ModelExtendedManagedFactory.h"
#include "TextureFactory.h"
#include "RenderSceneInfoCBuffer.h"


class DeferredBaseShader;
class DeferredComposeShader;
class DeferredRenderer;
class BufferFactory;
class Mesh;
struct Model;
struct Texture;
class ParticleRenderer;
class AglParticleSystem;
struct LightInstanceData;

class GraphicsWrapper
{
public:
	GraphicsWrapper(HWND p_hWnd, int p_width, int p_height, bool p_windowed);
	virtual ~GraphicsWrapper();
	///-----------------------------------------------------------------------------------
	/// Clears the back buffer with a non black color
	/// \return void
	///-----------------------------------------------------------------------------------
	void clearRenderTargets();

	///-----------------------------------------------------------------------------------
	/// Sets up the frame, prepares the renderer for draw calls.
	/// \return void
	///-----------------------------------------------------------------------------------
	void mapSceneInfo();

	///-----------------------------------------------------------------------------------
	/// Renders the whole scene
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderMesh(unsigned int p_meshId,vector<InstanceData>* p_instanceList);

	///-----------------------------------------------------------------------------------
	/// Set the current rasterizer state. By default it will allow to be overriden by the 
	/// wireframe mode setting.
	/// \param p_state
	/// \param p_allowWireframeModeOverride If true: will ignore change if global 
	/// wireframe mode is on. Will ignore wireframe mode if set to false. True by default.
	/// \return void
	///-----------------------------------------------------------------------------------
	void setRasterizerStateSettings(RasterizerState::Mode p_state, 
									bool p_allowWireframeModeOverride=true);

	void setBlendStateSettings( BlendState::Mode p_state );

	void setScissorRegion(int x, int y, int width, int height);

	void setPrimitiveTopology( PrimitiveTopology::Mode p_state);

	void setBaseRenderTargets();

	void setFinalBackbufferAsRenderTarget();
	///-----------------------------------------------------------------------------------
	/// Render compiled rocket geometry. Use this with libRocket so that the correct
	/// shader is used.
	/// \param p_mesh
	/// \param p_texture
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderGUIMesh( unsigned int p_meshId, vector<InstanceData>* p_instanceList );

	///-----------------------------------------------------------------------------------
	/// Finalizes the frame. For example; a deferred subsystem will
	/// render to backbuffer here.
	/// \return void
	///-----------------------------------------------------------------------------------
	void mapGBuffersToShader();
	void renderLights( LightMesh* p_mesh, vector<LightInstanceData>* p_instanceList );
	///-----------------------------------------------------------------------------------
	/// Switch the back buffer so that the current render target is presented
	/// \return void
	///-----------------------------------------------------------------------------------
	void flipBackBuffer();


	ModelResource* createModelFromFile(const string& p_name,
							   const string* p_path);

	vector<ModelResource*>* createModelsFromFile(const string& p_name,
		const string* p_path);

	// This is the preferred method for creating meshes from raw data
	unsigned int createMeshFromRaw(const string& p_name,
		int p_numVertices, PNTTBVertex* p_vertices, 
		int p_numIndices, DIndex* p_indices,
		int p_textureId);


	unsigned int createTexture(const string& p_name,
							   const string& p_path);
	unsigned int createTexture( const byte* p_source, int p_width, int p_height,
		int p_pitch, int p_bitLevel, TextureParser::TEXTURE_TYPE p_type );

	int getMeshId( const string& p_name );



	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void hookUpAntTweakBar();

	int getWindowWidth();
	int getWindowHeight();

	pair<int,int> getScreenPixelPosFromNDC(float p_x, float p_y);
	pair<int,int> getScreenPixelPosFromNDC(float p_x, float p_y, int p_width, int p_height);
	pair<float,float> getNDCPosFromScreenPixel(int p_x, int p_y);

	void changeBackbufferRes( int p_width, int p_height );

	void changeToWindowed(bool p_windowed);

	///-----------------------------------------------------------------------------------
	/// Sets the global wireframe mode. Causes everything to be displayed in wireframe.
	/// Separate rasterizer change calls can force to ignore global wireframe mode 
	/// explicitly though.
	/// \param p_wireframe
	/// \return void
	///-----------------------------------------------------------------------------------
	void setWireframeMode(bool p_wireframe);

	///-----------------------------------------------------------------------------------
	/// Handles all the rendering of the particle systems.
	/// \param p_system
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderParticleSystem(AglParticleSystem* p_system);

	void updateRenderSceneInfo(const RendererSceneInfo& p_sceneInfo);
private:
	void initSwapChain(HWND p_hWnd);

	///-----------------------------------------------------------------------------------
	/// Initialize the graphic card, need input is a win32 window used to present the 
	/// render result
	/// \return void
	///-----------------------------------------------------------------------------------
	void initHardware();

	void releaseBackBuffer();

	///-----------------------------------------------------------------------------------
	/// Creates the true back buffer
	/// \return void
	///-----------------------------------------------------------------------------------
	void initBackBuffer();

	///-----------------------------------------------------------------------------------
	/// Desc
	/// \return void
	///-----------------------------------------------------------------------------------
	void initViewport();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
	DXGI_SWAP_CHAIN_DESC	m_swapChainDesc;
	IDXGISwapChain*			m_swapChain;
	D3D_FEATURE_LEVEL		m_featureLevel;

	DeferredRenderer*		m_deferredRenderer;

	ID3D11RenderTargetView* m_backBuffer;

	DeferredBaseShader*		m_deferredBaseShader;

	// Creation & storage
	BufferFactory*			m_bufferFactory;
	TextureFactory*			m_textureFactory;
	ModelExtendedManagedFactory*	m_modelFactory;

	ResourceManager<Mesh>*		m_meshManager;
	ResourceManager<Texture>*	m_textureManager;

	ParticleRenderer*		m_particleRenderer;

	RendererSceneInfo		m_renderSceneInfo;
	Buffer<RenderSceneInfoCBuffer>* m_renderSceneInfoBuffer;

	int m_height;
	int m_width;
	bool m_windowed;
	bool m_wireframeMode;
};