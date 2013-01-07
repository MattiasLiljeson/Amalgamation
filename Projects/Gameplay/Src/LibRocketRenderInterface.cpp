/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

//==================================================================
// DX11 conversion of DX9 sample code from libRocket. This has been 
// modified and is NOT the original. Use as you wish. Please Leave 
// this message and please don't say that it is your work. Follow 
// the ToS above!
//
// - BiceMaster
//==================================================================

#include "LibRocketRenderInterface.h"

#include <BufferFactory.h>
#include <DIndex.h>
#include <InstanceData.h>
#include <Mesh.h>
#include <PNTTBVertex.h>
#include <Texture.h>
#include <TextureParser.h>
#include <sstream>

LibRocketRenderInterface::LibRocketRenderInterface( GraphicsWrapper* p_wrapper )
{
	m_factory = new BufferFactory(p_wrapper->getDevice(), p_wrapper->getDeviceContext());
	m_wrapper = p_wrapper;

	m_NDCFrom2dMatrix = createWorldMatrix();


// 	D3D11_RASTERIZER_DESC rasterizerDesc;
// 	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
// 	rasterizerDesc.CullMode = D3D11_CULL_NONE;
// 	rasterizerDesc.FrontCounterClockwise = TRUE;	// Changed it from CounterClockwise false to true, since it otherwise will be culled
// 	rasterizerDesc.DepthClipEnable = TRUE;
// 	rasterizerDesc.AntialiasedLineEnable = FALSE;
// 	rasterizerDesc.MultisampleEnable = FALSE;
// 	rasterizerDesc.DepthBias = 0;
// 	rasterizerDesc.DepthBiasClamp = 0.0f;
// 	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
// 
// 	rasterizerDesc.ScissorEnable = false;
// 	m_wrapper->getDevice()->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOff);
// 
// 	rasterizerDesc.ScissorEnable = true;
// 	m_wrapper->getDevice()->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOn);
// 
// 	m_wrapper->getDeviceContext()->RSSetState(rs_scissorsOff);
}

LibRocketRenderInterface::~LibRocketRenderInterface()
{
	delete m_factory;
	Rocket::Core::Shutdown();

	// SAFE_RELEASE(rs_scissorsOn);
	// SAFE_RELEASE(rs_scissorsOff);
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void LibRocketRenderInterface :: RenderGeometry(
	Rocket::Core::Vertex* ROCKET_UNUSED(vertices),
	int ROCKET_UNUSED(num_vertices),
	int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices),
	const Rocket::Core::TextureHandle ROCKET_UNUSED(texture),
	const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
	// We've chosen to not support non-compiled geometry in the DirectX renderer. If you wanted to render non-compiled
	// geometry, for example for very small sections of geometry, you could use DrawIndexedPrimitiveUP or write to a
	// dynamic vertex buffer which is flushed when either the p_texture changes or compiled geometry is drawn.
}

// Called by Rocket when it wants to compile geometry it believes will be static for the foreseeable future.
Rocket::Core::CompiledGeometryHandle LibRocketRenderInterface :: CompileGeometry(
	Rocket::Core::Vertex* p_vertices, int p_numVertices, int* p_indices, int p_numIndices,
	Rocket::Core::TextureHandle p_texture)
{
	//HACK: NOT DELETED NOW! MUST BE DONE!
	// Construct a new RocketD3D9CompiledGeometry structure, which will be returned as the handle, and the buffers to store the geometry.
	RocketCompiledGeometry* geometry = new RocketCompiledGeometry;

	// Fill the vertex vector.
	VertexType vertex;
	vector<VertexType> vertices;
	vertices.resize( p_numVertices );
	const int x=0, y=1, z=2;
	for (int i = 0; i < p_numVertices; i++)
	{
		vertex.pos[x] = p_vertices[i].position.x;
		vertex.pos[y] = p_vertices[i].position.y;
		vertex.pos[z] = 0;

		vertex.texCoord[x] = p_vertices[i].tex_coord[x];
		vertex.texCoord[y] = p_vertices[i].tex_coord[y];
		
		// No support for vertex colouring
		//vertex.color = D3DXCOLOR(p_vertices[i].colour.red, p_vertices[i].colour.green,
		//p_vertices[i].colour.blue, p_vertices[i].colour.alpha);

		vertices[i] = vertex;
	}

	// Fill the index vector.
	vector<DIndex> indices;
	indices.resize(p_numIndices);
	for( int i=0; i<p_numIndices; i++)
	{
		indices[i].index = p_indices[i];
	}

	//HACK: static int to enumerate menus
	static int numMenus = 0;
	numMenus++;
	stringstream ss;
	ss<<"menus nr: "<<numMenus;

	// HACK: Texture pointers should not be used. Instead the texture should be
	// created using CreateTexture in GraphicsWrapper, and then its id should be
	// passed along here instead.
	geometry->meshId = m_wrapper->createMesh(ss.str(), 
											 p_numVertices,&vertices[0], 
											 p_numIndices, &indices[0],
											 (Texture*)p_texture); // <-- change this to texture id (int)



	// geometry->meshId = m_wrapper->registerMesh( ss.str(), mesh, (Texture*)p_texture );

	return (Rocket::Core::CompiledGeometryHandle)geometry;
}

// Called by Rocket when it wants to render application-compiled geometry.
void LibRocketRenderInterface :: RenderCompiledGeometry(
	Rocket::Core::CompiledGeometryHandle p_geometry,
	const Rocket::Core::Vector2f& p_translation)
{
	RocketCompiledGeometry* geometry = (RocketCompiledGeometry*)p_geometry;

	// Build and set the transform matrix.
	AglVector3 translationVec = AglVector3( p_translation.x, p_translation.y, 0.0f );
	AglMatrix worldMat = AglMatrix::createTranslationMatrix( translationVec );
	worldMat *= m_NDCFrom2dMatrix;

	worldMat = worldMat.transpose();

	RendererSceneInfo scene;
	for( int i=0; i<16; i++ )
		scene.viewProjectionMatrix[i] = worldMat[i];

	m_wrapper->setSceneInfo(scene);
	m_wrapper->updatePerFrameConstantBuffer();
	m_wrapper->renderGUIMesh( geometry->meshId, &instanceDataVectorFromMatrix(worldMat) );
}

// Called by Rocket when it wants to release application-compiled geometry.
void LibRocketRenderInterface :: ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle p_geometry)
{
	RocketCompiledGeometry* geometry = (RocketCompiledGeometry*) p_geometry;
	delete geometry;
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void LibRocketRenderInterface :: EnableScissorRegion(bool enable)
{
	if(enable == true)
		m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_CCW_SCISSOR,false);
	else
		m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_CCW,false);
}

// Called by Rocket when it wants to change the scissor region.
void LibRocketRenderInterface :: SetScissorRegion(int x, int y, int width, int height)
{
	m_wrapper->setScissorRegion(x,y,width,height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a p_texture is required by the library.
bool LibRocketRenderInterface :: LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (file_handle == NULL)
		return false;

	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by Rocket when a p_texture is required to be built from an internally-generated sequence of pixels.
bool LibRocketRenderInterface :: GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	//pitch: 4  = RGBA = 4 bytes
	int x, y, pitch;
	x = source_dimensions.x;
	y = source_dimensions.y;
	pitch = source_dimensions.x * 4;

	ID3D11ShaderResourceView* resource = TextureParser::createTexture( 
		m_wrapper->getDevice(), source, x, y, pitch, TextureParser::RGBA );
	Texture* texture = new Texture( resource );

	// Set the handle on the Rocket p_texture structure.
	texture_handle = (Rocket::Core::TextureHandle)texture;
	return true;
}

// Called by Rocket when a loaded p_texture is no longer required.
void LibRocketRenderInterface :: ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
//	Texture* texture = ((Texture*) texture_handle);
//	delete texture;
}

// Returns the native horizontal texel offset for the renderer.
float LibRocketRenderInterface :: GetHorizontalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

// Returns the native vertical texel offset for the renderer.
float LibRocketRenderInterface :: GetVerticalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

AglMatrix LibRocketRenderInterface::createWorldMatrix()
{
	//==================================================================
	// changing coordinate system from 2D screen-space to NDC
	//    0							        1
	// 0  o---o							o---o 1
	//	  |   |					=>		|	|
	//    o---o Screen Width		 -1 o---o
	//        Screen Height			   -1   
	//
	// - Mattias L
	//==================================================================

	AglMatrix matScale = AglMatrix::identityMatrix();
	AglMatrix matTranslate = AglMatrix::identityMatrix();

	int wndWidth = m_wrapper->getWindowWidth();
	int wndHeight = m_wrapper->getWindowdHeight();

	// Flip Y-axis
	AglVector3 scale( 2.0f/wndWidth, -2.0f/wndHeight, 1.0f );
	AglVector3 translation( -1.0f, 1.0f, 0.0f );

	AglMatrix::componentsToMatrix(matTranslate, scale, AglQuaternion::identity(), translation);
	return matTranslate;
}

vector<InstanceData> LibRocketRenderInterface::instanceDataVectorFromMatrix( const AglMatrix& p_matrix )
{
	// Transpose matrix to col-major from row major as that is what the shader wants
	AglMatrix transMat = p_matrix.transpose();

	// Create Instance Data struct with World matrix used to render using instancing
	InstanceData data;
	for( int i=0; i<16; i++ )
	{
		data.worldTransform[i] = transMat[i];
	}
	vector<InstanceData> instances;
	instances.push_back( data );

	return instances;
}
