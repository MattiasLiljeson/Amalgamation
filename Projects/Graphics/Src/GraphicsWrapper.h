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
#include "ConnectionPointCollection.h"
#include "TextureParser.h"


class DeferredBaseShader;
class DeferredComposeShader;
class DeferredRenderer;
class BufferFactory;
class Mesh;
struct Texture;
class ParticleRenderer;
class AglParticleSystem;

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
	/// Passes the scene info(world-view-projection matrix for example) 
	/// to the render subsystem.
	/// \param p_sceneInfo
	/// \return void
	///-----------------------------------------------------------------------------------
	void setSceneInfo(const RendererSceneInfo& p_sceneInfo);

	///-----------------------------------------------------------------------------------
	/// Sets up the frame, prepares the renderer for draw calls.
	/// \return void
	///-----------------------------------------------------------------------------------
	void beginFrame();

	///-----------------------------------------------------------------------------------
	/// HACK: Temporary function to update the per frame CB in the middle of the frame
	/// \return void
	///-----------------------------------------------------------------------------------
	void updatePerFrameConstantBuffer();

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

	void setScissorRegion(int x, int y, int width, int height);

	///-----------------------------------------------------------------------------------
	/// Render compiled rocket geometry. Use this with libRocket so that the correct
	/// shader is used.
	/// \param p_mesh
	/// \param p_texture
	/// \return void
	///-----------------------------------------------------------------------------------
	void beginGUIPass();
	void renderGUIMesh( unsigned int p_meshId,
		vector<InstanceData>* p_instanceList );
	void finalizeGUIPass();

	///-----------------------------------------------------------------------------------
	/// Finalizes the frame. For example; a deferred subsystem will
	/// render to backbuffer here.
	/// \return void
	///-----------------------------------------------------------------------------------
	void finalizeFrame();

	///-----------------------------------------------------------------------------------
	/// Switch the back buffer so that the current render target is presented
	/// \return void
	///-----------------------------------------------------------------------------------
	void flipBackBuffer();

	///-----------------------------------------------------------------------------------
	/// Create a mesh using name (and loads if path is specified). Returns a mesh id.
	/// \param p_name
	/// \param p_path
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	///-----------------------------------------------------------------------------------
	/// Create a mesh using name (and loads if path is specified). Returns a mesh id.
	/// \param p_name Filename
	/// \param p_path Path, without filename
	/// \param p_outHardPoints Optional container for storing connection points.
	/// \return unsigned int Mesh id
	///-----------------------------------------------------------------------------------
	unsigned int createMesh(const string& p_name,
							const string* p_path=NULL,
							ConnectionPointCollection* p_outConnectionPoints=NULL);

	// WIP, should not use texture pointer, but texture id
	unsigned int createMesh(const string& p_name,
							int p_numVertices, PNTTBVertex* p_vertices, 
							int p_numIndices, DIndex* p_indices,
							Texture* p_texture=NULL);

	// This is the preferred method for creating meshes from raw data
	unsigned int createMesh(const string& p_name,
							int p_numVertices, PNTTBVertex* p_vertices, 
							int p_numIndices, DIndex* p_indices,
							int p_textureId);

	unsigned int createTexture(const string& p_name,
							   const string& p_path);
	unsigned int createTexture( const byte* p_source, int p_width, int p_height,
		int p_pitch, TextureParser::TEXTURE_TYPE p_type );

	int getMeshId( const string& p_name );


	// HACK: Pointer to texture should not be used. A texture id should be used instead.
	///-----------------------------------------------------------------------------------
	/// WIP! Decide how to handle this when several textures/materials are present.
	/// Should texture even be sent in here??
	/// Register an externally created mesh in the graphics system
	/// \param p_name
	/// \param p_mesh
	/// \param p_texture
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	unsigned int registerMesh( const string& p_name, Mesh* p_mesh, Texture* p_texture );


	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void hookUpAntTweakBar();

	int getWindowWidth();
	int getWindowHeight();
	pair<int,int> getScreenPixelPosFromNDC(float p_x,float p_y);

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
	/// NOT IMPLEMENTED
	/// \return void
	///-----------------------------------------------------------------------------------
	void beginParticleRender();

	///-----------------------------------------------------------------------------------
	/// Handles all the rendering of the particle systems.
	/// \param p_system
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderParticleSystem(AglParticleSystem* p_system);

	///-----------------------------------------------------------------------------------
	/// NOT IMPLEMENTED
	/// \return void
	///-----------------------------------------------------------------------------------
	void endParticleRender();

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

	ResourceManager<Mesh>*		m_meshManager;
	ResourceManager<Texture>*	m_textureManager;

	ParticleRenderer*		m_particleRenderer;

	RendererSceneInfo		m_renderSceneInfo;

	int m_height;
	int m_width;
	bool m_windowed;
	bool m_wireframeMode;
};