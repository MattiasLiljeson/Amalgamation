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

//=============================================================================
// DX11 conversion of DX9 sample code from libRocket. This has been modified
// and is NOT the original. The same license applies for the changed code as
// for the original.
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
#include <Rocket/Core/Vertex.h>

LibRocketRenderInterface::LibRocketRenderInterface( GraphicsWrapper* p_wrapper )
{
	m_factory = new BufferFactory(p_wrapper->getDevice(), p_wrapper->getDeviceContext());
	m_wrapper = p_wrapper;

	updateOnWindowResize();
	numCompiledGeometries = 0;

	byte fallbackTexture[4] = { 255, 255, 255, 255 };
	m_stdTextureId = m_wrapper->createTexture( fallbackTexture, 1, 1, 4, 32,
		TextureParser::RGBA);
}

LibRocketRenderInterface::~LibRocketRenderInterface()
{
	delete m_factory;
	Rocket::Core::Shutdown();

	for( unsigned int i=0; i<m_compiledGeometries.size(); i++ )
	{
		delete m_compiledGeometries[i];
		m_compiledGeometries[i] = NULL;
	}
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

// Called by Rocket when it wants to compile geometry it believes will be static for 
// the foreseeable future.
Rocket::Core::CompiledGeometryHandle LibRocketRenderInterface :: CompileGeometry(
	Rocket::Core::Vertex* p_vertices, int p_numVertices, int* p_indices, int p_numIndices,
	Rocket::Core::TextureHandle p_texture)
{
	numCompiledGeometries++;
	// Construct a new RocketCompiledGeometry structure, which will be returned as the
	// handle, and the buffers to store the geometry.
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

		vertex.normal[x] = p_vertices[i].colour.red;
		vertex.normal[y] = p_vertices[i].colour.green;
		vertex.normal[z] = p_vertices[i].colour.blue;
		vertex.pos[z]	 = p_vertices[i].colour.alpha;

		vertex.texCoord[x] = p_vertices[i].tex_coord[x];
		vertex.texCoord[y] = p_vertices[i].tex_coord[y];
		
		vertices[i] = vertex;
	}

	// Fill the index vector.
	vector<DIndex> indices;
	indices.resize(p_numIndices);
	for( int i=0; i<p_numIndices; i++)
	{
		indices[i].index = p_indices[i];
	}

	
	// Make sure to use the std tex if no texture is defined
	if( p_texture == 0){
		p_texture = 1;
	}

	stringstream ss;
	ss<<"menus nr: "<<numCompiledGeometries;
	geometry->meshId = m_wrapper->createMeshFromRaw(ss.str(), 
											 p_numVertices,&vertices[0], 
											 p_numIndices, &indices[0],
											 static_cast<int>(p_texture));

	m_compiledGeometries.push_back( geometry );

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
	scene.viewProj = worldMat;

	m_wrapper->updateRenderSceneInfo(scene);
	m_wrapper->mapSceneInfo();
	m_wrapper->renderGUIMeshList( geometry->meshId, &instanceDataVectorFromMatrix(worldMat) );
}

// Called by Rocket when it wants to release application-compiled geometry.
void LibRocketRenderInterface :: ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle p_geometry)
{
	RocketCompiledGeometry* geometry = (RocketCompiledGeometry*) p_geometry;

	vector<RocketCompiledGeometry*>::iterator it = m_compiledGeometries.begin();
	for( ; it != m_compiledGeometries.end(); it++ )
	{
		if( (*it) == geometry )
		{
			m_compiledGeometries.erase(it);
			break;
		}
	}
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
bool LibRocketRenderInterface :: LoadTexture(Rocket::Core::TextureHandle& texture_handle,
	Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	bool fileFound = false;
	int slashPos = source.RFind("/", source.Length());
	if( slashPos != -1) //slash found
	{
		// Split string
		Rocket::Core::String& path = source.Substring( 0, slashPos );
		Rocket::Core::String& file = source.Substring( slashPos, source.Length() );

		// Fetch it
		texture_handle = (unsigned int)m_wrapper->createTexture( string(file.CString()),
			string(path.CString()) );

		fileFound = true;
	}
	else
	{
		// Use std texture
		texture_handle = 0;
		fileFound = false;
	}

	return fileFound;
}

// Called by Rocket when a p_texture is required to be built from an internally-generated sequence of pixels.
bool LibRocketRenderInterface :: GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
	const byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	//pitch: 4  = RGBA = 4 bytes
	int pitch = source_dimensions.x * 4;

	unsigned int texId = m_wrapper->createTexture(
		source, source_dimensions.x, source_dimensions.y, pitch, 32, TextureParser::RGBA );

	// Set the handle on the Rocket p_texture structure.
	texture_handle = (Rocket::Core::TextureHandle)texId;
	return true;
}

// Called by Rocket when a loaded p_texture is no longer required.
void LibRocketRenderInterface :: ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	// Texture are not allocated here but just function as indices to the graphics wrapper.
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
	int wndHeight = m_wrapper->getWindowHeight();

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

void LibRocketRenderInterface::updateOnWindowResize()
{
	m_NDCFrom2dMatrix = createWorldMatrix();
}
