#include "GraphicsWrapper.h"
#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "DeferredRenderer.h"
#include "Mesh.h"
#include "BufferFactory.h"
#include "AntTweakBarWrapper.h"
#include "TextureParser.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshLoadException.h"

#include <AglReader.h>
#include <FileCheck.h>
#include "D3DException.h"
#include "D3DUtil.h"
#include <AglStandardParticle.h>

GraphicsWrapper::GraphicsWrapper(HWND p_hWnd, int p_width, int p_height, bool p_windowed)
{
	m_device		= NULL;
	m_deviceContext = NULL;
	m_swapChain		= NULL;

	m_deferredBaseShader	= NULL;

	m_width	= p_width;
	m_height= p_height;
	m_windowed = p_windowed;
	m_wireframeMode = false;

	initSwapChain(p_hWnd);
	initHardware();

	initBackBuffer();
	initViewport();

	m_bufferFactory		= new BufferFactory(m_device,m_deviceContext);
	m_meshManager		= new ResourceManager<Mesh>();
	m_textureManager	= new ResourceManager<Texture>();
	createTexture("mesherror.png",TEXTUREPATH);

	m_deferredRenderer = new DeferredRenderer( m_device, m_deviceContext, 
							   m_width, m_height);

	clearRenderTargets();
}

GraphicsWrapper::~GraphicsWrapper()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	releaseBackBuffer();
	
	delete m_deferredRenderer;
	delete m_deferredBaseShader;
	delete m_bufferFactory;
	delete m_meshManager;
	delete m_textureManager;
}

void GraphicsWrapper::initSwapChain(HWND p_hWnd)
{
	ZeroMemory( &m_swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC) );
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = m_width;
	m_swapChainDesc.BufferDesc.Height = m_height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = p_hWnd;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.Windowed = m_windowed;
}

void GraphicsWrapper::initHardware()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	int selectedDriverType = -1;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		D3D_DRIVER_TYPE driverType;
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&m_swapChainDesc,
			&m_swapChain,
			&m_device,
			&initiatedFeatureLevel,
			&m_deviceContext);
		
		if (hr == S_OK)
		{
			selectedDriverType = driverTypeIndex;
			break;
		}
	}
	if ( selectedDriverType > 0 )
		throw D3DException("Couldn't create a D3D Hardware-device, software render enabled."
		,__FILE__, __FUNCTION__, __LINE__);
}

void GraphicsWrapper::initBackBuffer()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* backBufferTexture;

	hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), 
		(LPVOID*)&backBufferTexture );

	if( FAILED(hr) )
		throw D3DException("Failed to get backbuffer from swap chain.",__FILE__,
		__FUNCTION__,__LINE__);

	hr = m_device->CreateRenderTargetView( backBufferTexture, NULL, &m_backBuffer );
	backBufferTexture->Release();
	if( FAILED(hr) )
		throw D3DException("Failed to create rendertargetview from back buffer.",
		__FILE__,__FUNCTION__,__LINE__);
}

void GraphicsWrapper::clearRenderTargets()
{
	m_deferredRenderer->clearBuffers();
	
	static float ClearColor[4] = { 1, 0, 0, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_backBuffer,ClearColor);
}

void GraphicsWrapper::setSceneInfo(const RendererSceneInfo& p_sceneInfo)
{
	m_deferredRenderer->setSceneInfo(p_sceneInfo);
}

void GraphicsWrapper::beginFrame()
{
	setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_deferredRenderer->beginDeferredBasePass();
}

void GraphicsWrapper::updatePerFrameConstantBuffer()
{
	m_deferredRenderer->updatePerFrameConstantBuffer();
}

void GraphicsWrapper::renderMesh(unsigned int p_meshId,
								 vector<InstanceData>* p_instanceList)
{
	Mesh* mesh = m_meshManager->getResource(p_meshId);
	
	/************************************************************************/
	/* Get the size of the materials and use it to create a new texture		*/
	/* array.																*/
	/************************************************************************/
	unsigned int arraySize = mesh->getMaterialInfo().SIZE;
	Texture** textureArray = new Texture*[arraySize];
	for (unsigned int i = 0; i < arraySize; i++)
	{
		unsigned int textureId;
		textureId = mesh->getMaterialInfo().getTextureType((MaterialInfo::TextureTypes)i);

		/************************************************************************/
		/* Check if the texture ID is active and get the texture resource or	*/
		/* set the value in the texture array to NULL							*/
		/************************************************************************/
		if(textureId != 0)
			textureArray[i] = m_textureManager->getResource(textureId);
		else
			textureArray[i] = NULL;
	}

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer(&(*p_instanceList)[0],
														   p_instanceList->size());
	m_deferredRenderer->renderMeshInstanced(mesh, textureArray, arraySize, instanceBuffer);

	delete textureArray;
	delete instanceBuffer;
}

void GraphicsWrapper::setRasterizerStateSettings(RasterizerState::Mode p_state, 
												 bool p_allowWireframeModeOverride)
{
	RasterizerState::Mode state = m_deferredRenderer->getCurrentRasterizerStateType();
	// accept rasterizer state change if not in wireframe mode or 
	// if set to not allow wireframe mode
	if (!m_wireframeMode || !p_allowWireframeModeOverride)
		m_deferredRenderer->setRasterizerStateSettings(p_state);
	else if (state != RasterizerState::WIREFRAME) 
	{   
		// otherwise, force wireframe(if not already set)
		m_deferredRenderer->setRasterizerStateSettings(RasterizerState::WIREFRAME);
	}
}


void GraphicsWrapper::setScissorRegion( int x, int y, int width, int height )
{
	D3D11_RECT scissor_rect;
	scissor_rect.left = x;
	scissor_rect.right = x + width;
	scissor_rect.top = y;
	scissor_rect.bottom = y + height;

	m_deviceContext->RSSetScissorRects(1, &scissor_rect);
}

void GraphicsWrapper::beginGUIPass()
{
	m_deferredRenderer->beginGUIPass();
}

void GraphicsWrapper::renderGUIMesh( unsigned int p_meshId, 
									 vector<InstanceData>* p_instanceList )
{
	Mesh* mesh = m_meshManager->getResource( p_meshId );
	Texture* tex = m_textureManager->getResource( 
		mesh->getMaterialInfo().getTextureType(MaterialInfo::DIFFUSEMAP) );

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer( &(*p_instanceList)[0],
		p_instanceList->size() );

	m_deferredRenderer->renderGUIMesh( mesh, tex );

	delete instanceBuffer;
}

void GraphicsWrapper::finalizeGUIPass()
{
	m_deferredRenderer->finalizeGUIPass();
}

void GraphicsWrapper::finalizeFrame()
{
	setRasterizerStateSettings(RasterizerState::DEFAULT,false);
	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, NULL);
	m_deferredRenderer->renderComposedImage();
}

void GraphicsWrapper::flipBackBuffer()
{
	m_swapChain->Present( 0, 0);
}

unsigned int GraphicsWrapper::createMesh( const string& p_name,
										  const string* p_path/*=NULL*/,
										  ConnectionPointCollection* p_outConnectionPoints/*=NULL*/)
{
	// =============================================
	//
	// WORK IN PROGRESS.
	// Will need refactoring.
	//
	// =============================================
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		// =============================================
		// PRIMITIVES
		// =============================================
		if (p_name=="P_cube")
		{
			MaterialInfo materialInfo;
			Mesh* mesh = m_bufferFactory->createBoxMesh(); // construct a mesh
			meshResultId = m_meshManager->addResource(p_name,mesh);	   // put in manager
			// (Here you might want to do similar checks for textures/materials
			// For now we have a hard coded texture path, but later on
			// we probably get this path from a mesh file loader or similar.
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
				createTexture("10x10.png",TESTTEXTUREPATH));
			materialInfo.setTextureId(MaterialInfo::NORMALMAP,
				createTexture("testtexture.png",TESTTEXTUREPATH));
			// and their managers.)
			// ...
			// and then set the resulting data to the mesh
			mesh->setTextureId(materialInfo);
		}
		else if (p_name=="P_sphere")
		{
			MaterialInfo materialInfo;
			Mesh* mesh = m_bufferFactory->createSphereMesh(); // construct a mesh
			meshResultId = m_meshManager->addResource(p_name,mesh);	   // put in manager
			// (Here you might want to do similar checks for textures/materials
			// For now we have a hard coded texture path, but later on
			// we probably get this path from a mesh file loader or similar.
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
				createTexture("10x10.png",TESTTEXTUREPATH));
			materialInfo.setTextureId(MaterialInfo::NORMALMAP,
				createTexture("testtexture.png",TESTTEXTUREPATH));
			// and their managers.)
			// ...
			// and then set the resulting data to the mesh
			mesh->setTextureId(materialInfo);
		}
		else
		// =============================================
		// MODEL FILES
		// =============================================
		{
			// Construct path for loading
			string fullPath;
			if (p_path!=NULL) fullPath = *p_path;
			fullPath += p_name;
			// test file
			string fileChkMsg;
			if (!isFileOk(fullPath,fileChkMsg,__FILE__,__FUNCTION__,__LINE__))
				throw MeshLoadException(fileChkMsg);
			// read file and extract scene
			AglReader meshReader(fullPath.c_str());
			AglScene* aglScene = meshReader.getScene();
			//
			if (aglScene)
			{ 
				// ------------------
				// Mesh
				// ------------------
				// only handle one mesh for now.
				AglMesh* aglMesh = aglScene->getMeshes()[0];
				AglMeshHeader aglMeshHeader = aglMesh->getHeader();
				MaterialInfo materialInfo;
				// Raw data extraction
				void* vertices = aglMesh->getVertices();
				void* indices = static_cast<void*>(aglMesh->getIndices());
				unsigned int numVertices = static_cast<unsigned int>(aglMeshHeader.vertexCount);
				unsigned int numIndices =  static_cast<unsigned int>(aglMeshHeader.indexCount);
				// Internal mesh format creation
				Mesh* mesh = m_bufferFactory->createMeshFromRaw(vertices, indices,
																numVertices,
																numIndices);
				// put in manager
				meshResultId = m_meshManager->addResource(p_name,mesh);	
				// (Here you might want to do similar checks for textures/materials
				// For now we have a hard coded texture path, but later on
				// we probably get this path from a mesh file loader or similar.
				materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, 
					createTexture("testtexture.png",TESTTEXTUREPATH));
				materialInfo.setTextureId(MaterialInfo::NORMALMAP,
					createTexture("testtexture.png",TESTTEXTUREPATH));
				// and their managers.)
				// ...
				// and then set the resulting data to the mesh
				mesh->setTextureId(materialInfo);

				// ------------------
				// Connection points
				// ------------------
				if (p_outConnectionPoints!=NULL)
				{
					for (unsigned int i=0;i<aglScene->getConnectionPointCount();i++)
					{
						RawTransformData dat;
						AglMatrix mat = aglScene->getConnectionPoint(i).transform;
						for (unsigned int n=0;n<16;n++)
							dat.transform[n] = mat.data[n];
						p_outConnectionPoints->m_collection.push_back(dat);
					}
				}
			}
			else
			{
				// fallback mesh and texture
				Mesh* mesh = m_bufferFactory->createBoxMesh();
				MaterialInfo materialInfo;
				meshResultId = m_meshManager->addResource(p_name,mesh);
				materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
					createTexture("mesherror.png",TEXTUREPATH));
				materialInfo.setTextureId(MaterialInfo::NORMALMAP,
					createTexture("testtexture.png",TESTTEXTUREPATH));
				mesh->setTextureId(materialInfo);
			}
			// cleanup
			delete aglScene;
		}
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
		Mesh* mesh = m_meshManager->getResource(meshResultId); // get mesh from id
	}
	return meshResultId;
}

unsigned int GraphicsWrapper::createMesh( const string& p_name, 
										 int p_numVertices, PNTTBVertex* p_vertices, 
										 int p_numIndices, DIndex* p_indices, 
										 Texture* p_texture/*=NULL*/ )
{
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		Mesh* mesh = m_bufferFactory->createMeshFromPNTTBVerticesAndIndices( p_numVertices,
		p_vertices, p_numIndices, p_indices );

		meshResultId = registerMesh( p_name, mesh, p_texture ); // HACK: textures should be handled 
																// by index instead
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
	}
	return meshResultId;
}



unsigned int GraphicsWrapper::createMesh( const string& p_name, 
										 int p_numVertices, PNTTBVertex* p_vertices, 
										 int p_numIndices, DIndex* p_indices, 
										 int p_textureId )
{
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		Mesh* mesh = m_bufferFactory->createMeshFromPNTTBVerticesAndIndices( p_numVertices,
			p_vertices, p_numIndices, p_indices );
		meshResultId = (int)m_meshManager->addResource( p_name, mesh );

		if( p_textureId != -1 )
		{
			MaterialInfo materialInfo;
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, p_textureId);
			mesh->setTextureId( materialInfo );
		}
		
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
	}
	return meshResultId;
}


unsigned int GraphicsWrapper::registerMesh( const string& p_name, Mesh* p_mesh, 
										   Texture* p_texture )
{
	// check if resource already exists
	int meshId = m_meshManager->getResourceId( p_name );
	if( meshId == -1 ) 
	{
		meshId = (int)m_meshManager->addResource( p_name, p_mesh );

		string textureName = p_name + "_tex";
		int texId = m_textureManager->getResourceId( p_texture );
		if( texId == -1 )
		{
			texId = static_cast<int>(m_textureManager->addResource( textureName, 
				p_texture ));
		}
		MaterialInfo materialInfo;
		materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, texId);
		p_mesh->setTextureId( materialInfo );
	}
	return meshId;
}

unsigned int GraphicsWrapper::createTexture( const string& p_name, 
											 const string& p_path)
{
	int texFoundId = m_textureManager->getResourceId(p_name);
	unsigned int texResultId = 0;

	if (texFoundId==-1)  // if it does not exist, create new
	{
		Texture* tex;
		tex = new Texture(TextureParser::loadTexture(m_device,
			(p_path+p_name).c_str()) );
		texResultId = m_textureManager->addResource(p_name,tex);
	}
	else
	{
		texResultId = static_cast<unsigned int>(texFoundId);
	}
	return texResultId;
}

unsigned int GraphicsWrapper::createTexture( const byte* p_source, int p_width,
	int p_height, int p_pitch, TextureParser::TEXTURE_TYPE p_type )
{
	// Create texture name used by manager
	static int createdTextureCount = 0;
	createdTextureCount++;
	stringstream ss;
	ss<<"Created Texture "<< createdTextureCount;
	string textureName = ss.str();
	
	// Create texture
	Texture* tex = new Texture(
		TextureParser::createTexture( m_device, p_source, p_width, p_height, p_pitch, p_type) );
	int textureId = m_textureManager->addResource( textureName, tex );
	return textureId;
}

int GraphicsWrapper::getMeshId( const string& p_name )
{
	return m_meshManager->getResourceId(p_name);
}

void GraphicsWrapper::initViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (float)m_width;
	vp.Height	= (float)m_height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_deviceContext->RSSetViewports(1, &vp);
}

ID3D11Device* GraphicsWrapper::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* GraphicsWrapper::getDeviceContext()
{
	return m_deviceContext;
}

void GraphicsWrapper::hookUpAntTweakBar()
{
	m_deferredRenderer->hookUpAntTweakBar();
}

int GraphicsWrapper::getWindowWidth()
{
	return m_width;
}

int GraphicsWrapper::getWindowHeight()
{
	return m_height;
}

pair<int,int> GraphicsWrapper::getScreenPixelPosFromNDC( float p_x, float p_y )
{
	// NDC -> Screenspace
	int screenSpaceX = static_cast<int>((p_x+1.0f)/2 * (m_width));
	int screenSpaceY = static_cast<int>((p_y+1.0f)/2 * (m_height));
	return pair<int,int>(screenSpaceX,screenSpaceY);
}

void GraphicsWrapper::changeBackbufferRes( int p_width, int p_height ){
	m_width = p_width;
	m_height = p_height;

	m_deviceContext->OMSetRenderTargets(0, 0, 0);

	releaseBackBuffer();
	m_deferredRenderer->releaseRenderTargetsAndDepthStencil();

	HRESULT hr;
	// Resize swap chain to window's size.
	hr = m_swapChain->ResizeBuffers(0, p_width, p_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if(FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	initBackBuffer();
	initViewport();

	m_deferredRenderer->initRendertargetsAndDepthStencil( m_width, m_height );
}

void GraphicsWrapper::releaseBackBuffer()
{
	SAFE_RELEASE( m_backBuffer );
}
void GraphicsWrapper::changeToWindowed( bool p_windowed )
{
	HRESULT hr = S_OK;
	m_windowed = !p_windowed;
	hr = m_swapChain->SetFullscreenState((BOOL)m_windowed,nullptr);
	if( FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
}

void GraphicsWrapper::setWireframeMode( bool p_wireframe )
{
	m_wireframeMode = p_wireframe;
}

void GraphicsWrapper::renderParticles( vector<AglStandardParticle>* p_data )
{
	/*
	ID3D11Buffer* buffer = NULL;

	if (p_data->size()>0)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(AglStandardParticle)*p_data->size();
		bd.BindFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &p_data[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		m_device->CreateBuffer(&bd, &vertexData, &buffer);
	}
	if(buffer)
	{
		ID3D11DepthStencilState* old;
		UINT stencil;

		//m_deviceContext->OMGetDepthStencilState(&old, &stencil);
		//m_deviceContext->OMSetBlendState(blendState, NULL, 0xFFFFFF);
		//m_deviceContext->OMSetDepthStencilState(depthStencil, 1);


	}
	*/
}



