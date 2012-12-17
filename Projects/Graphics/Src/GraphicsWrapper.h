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
#include "BufferFactory.h"
#include "AntTweakBarWrapper.h"
#include "D3DException.h"
#include "D3DUtil.h"
#include "Buffer.h"
#include "DeferredRenderer.h"
#include "RendererSceneInfo.h"
#include "TextureParser.h"
#include "Texture.h"

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
	/// Renders the whole scene
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderMesh(unsigned int p_meshId,vector<InstanceData>* p_instanceList);

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
	/// Create a mesh using name.
	/// Extend this with functionality for loading etc...
	/// \param p_name
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	unsigned int createMesh(const string& p_name);


	///-----------------------------------------------------------------------------------
	/// Used by libRocket //Mattias
	/// \param p_name
	/// \param p_mesh
	/// \param p_texture
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	unsigned int createMesh( const string& p_name, Mesh* p_mesh, Texture* p_texture );

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void hookUpAntTweakBar();
private:

	///-----------------------------------------------------------------------------------
	/// Initialize the graphic card, need input is a win32 window used to present the 
	/// render result
	/// \param p_hWnd
	/// \param p_windowed
	/// \return void
	///-----------------------------------------------------------------------------------
	void initHardware(HWND p_hWnd, bool p_windowed);

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
	IDXGISwapChain*			m_swapChain;
	D3D_FEATURE_LEVEL		m_featureLevel;

	DeferredRenderer*		m_deferredRenderer;

	ID3D11RenderTargetView* m_backBuffer;

	DeferredBaseShader*		m_deferredBaseShader;

	// Creation & storage
	BufferFactory*			m_bufferFactory;

	ResourceManager<Mesh>*		m_meshManager;
	ResourceManager<Texture>*	m_textureManager;

	int m_height;
	int m_width;
};