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
// DX10 conversion of DX9 sample code from libRocket. This has been 
// modified and is NOT the original. Use as you wish. Please Leave 
// this message and please don't say that it is your work 
//
// - BiceMaster
//==================================================================

#include "RenderInterfaceDx10.h"

RenderInterfaceDx10::RenderInterfaceDx10(ID3D10Device* _device, ID3D10Effect* _effect, int _techNr, int _passNr)
{
	//device = _device;
	//effect = _effect;
	//techNr = _techNr;
	//passNr = _passNr;

	//D3D10_RASTERIZER_DESC rasterizerDesc;
	//rasterizerDesc.FillMode = D3D10_FILL_SOLID;
	//rasterizerDesc.CullMode = D3D10_CULL_BACK;
	//rasterizerDesc.FrontCounterClockwise = TRUE;	// Changed it from CounterClockwise false to true, since it otherwise will be culled
	//rasterizerDesc.DepthClipEnable = TRUE;
	//rasterizerDesc.AntialiasedLineEnable = FALSE;
	//rasterizerDesc.MultisampleEnable = FALSE;
	//rasterizerDesc.DepthBias = 0;
	//rasterizerDesc.DepthBiasClamp = 0.0f;
	//rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//rasterizerDesc.ScissorEnable = false;
	//HR(device->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOff));

	//rasterizerDesc.ScissorEnable = true;
	//HR(device->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOn));

	//device->RSSetState(rs_scissorsOff);

	//defineInputElementDesc();
	//defineBlendState();
	//initPipeline();
	//initFxVars();
}

RenderInterfaceDx10::~RenderInterfaceDx10()
{
	//SAFE_RELEASE(rs_scissorsOn);
	//SAFE_RELEASE(rs_scissorsOff);
	//SAFE_RELEASE(inputLayout);
	//delete layoutDesc;

	Rocket::Core::Shutdown();
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void RenderInterfaceDx10 :: RenderGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
	int korv = 1; //debug

	// We've chosen to not support non-compiled geometry in the DirectX renderer. If you wanted to render non-compiled
	// geometry, for example for very small sections of geometry, you could use DrawIndexedPrimitiveUP or write to a
	// dynamic vertex buffer which is flushed when either the texture changes or compiled geometry is drawn.
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
Rocket::Core::CompiledGeometryHandle RenderInterfaceDx10 :: CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
	int korv = 1; //debug

	// Construct a new RocketD3D9CompiledGeometry structure, which will be returned as the handle, and the buffers to store the geometry.
	RocketDx10CompiledGeometry* geometry = new RocketDx10CompiledGeometry;

	// Fill the vertex vector.
	VertexType _vertex;
	for (int i = 0; i < num_vertices; i++)
	{
		_vertex.pos = D3DXVECTOR3(vertices[i].position.x, vertices[i].position.y, 0);
		_vertex.color = D3DXCOLOR(vertices[i].colour.red, vertices[i].colour.green, vertices[i].colour.blue, vertices[i].colour.alpha);
		_vertex.texCoord = D3DXVECTOR2(vertices[i].tex_coord[0], vertices[i].tex_coord[1]);

		geometry->vertices.push_back(_vertex);
	}

	// Fill the index vector.
	geometry->indices.resize(num_indices);
	memcpy(&geometry->indices[0], indices, sizeof(unsigned int) * num_indices);

	//create and fill the vertex buffer
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DYNAMIC;
	vbd.ByteWidth = num_vertices*sizeof(VertexType);
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertBuffSubRes;
	vertBuffSubRes.pSysMem = &geometry->vertices[0];
	HR(device->CreateBuffer(&vbd, &vertBuffSubRes, &geometry->vertexBuffer));

	//create and fill the index buffer
	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_DYNAMIC;
	ibd.ByteWidth = num_indices*sizeof(int);
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indBuffSubRes;
	indBuffSubRes.pSysMem = &geometry->indices[0];
	HR(device->CreateBuffer(&ibd, &indBuffSubRes, &geometry->indexBuffer));

	//set misc
	geometry->num_vertices = (DWORD) num_vertices;
	geometry->num_primitives = (DWORD) num_indices / 3;
	geometry->texture = texture == NULL ? NULL : (Texture*) texture;

	return (Rocket::Core::CompiledGeometryHandle)geometry;
}

// Called by Rocket when it wants to render application-compiled geometry.
void RenderInterfaceDx10 :: RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
	int korv = 1; //debug

	RocketDx10CompiledGeometry* _geometry = (RocketDx10CompiledGeometry*)geometry;

	//set states
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	device->OMSetBlendState(bs_normal, blendFactor, 0xffffffff);

	// set which texture to use
	if(_geometry->texture != NULL)
		_geometry->texture->useTexture();

	// Build and set the transform matrix.
	D3DXMatrixTranslation(&mat_world, translation.x, translation.y, 0);
	
	//==================================================================
	// HACK below: changing coordinate system from NDC to:
	//    0%
	// 0% #---#
	//	  |   |
	//    #---# 100%
	//        100%
	// This should probably not have to be done. I'm not sure though.
	// I'll let this stay until I know for sure
	// - Mattias L

	D3DXMATRIX _mat_scale;
	D3DXMATRIX _mat_translate;

	D3DXMatrixIdentity(&_mat_scale); 
	D3DXMatrixIdentity(&_mat_translate);

	D3DXMatrixScaling(&_mat_scale , 2.0f/1280.0f, -2.0f/720.0f, 1); 
	D3DXMatrixTranslation(&_mat_translate, -1, 1, 0);
	
	mat_world = mat_world * _mat_scale * _mat_translate;
	//==================================================================

	HR(fxVar_world->SetMatrix(&mat_world._11));

	device->IASetInputLayout(inputLayout);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	device->IASetVertexBuffers(0, 1, &_geometry->vertexBuffer, &stride, &offset);
	device->IASetIndexBuffer(_geometry->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	HR(pass->Apply(passNr));

	// draw to the the back buffer
	device->DrawIndexed(_geometry->num_primitives*3, 0, 0);
}

// Called by Rocket when it wants to release application-compiled geometry.
void RenderInterfaceDx10 :: ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	int korv = 1; //debug

	RocketDx10CompiledGeometry* _geometry = (RocketDx10CompiledGeometry*) geometry;

	SAFE_RELEASE(_geometry->vertexBuffer);
	SAFE_RELEASE(_geometry->indexBuffer);

	//delete _geometry->texture;
	delete _geometry;
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void RenderInterfaceDx10 :: EnableScissorRegion(bool enable)
{
	if(enable == true)
		device->RSSetState(rs_scissorsOn);
	else
		device->RSSetState(rs_scissorsOff);
}

// Called by Rocket when it wants to change the scissor region.
void RenderInterfaceDx10 :: SetScissorRegion(int x, int y, int width, int height)
{
	D3D10_RECT scissor_rect;
	scissor_rect.left = x;
	scissor_rect.right = x + width;
	scissor_rect.top = y;
	scissor_rect.bottom = y + height;

	device->RSSetScissorRects(1, &scissor_rect);
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

// Called by Rocket when a texture is required by the library.
bool RenderInterfaceDx10 :: LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
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

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool RenderInterfaceDx10 :: GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	Texture* _texture = new Texture;
	_texture->setDeviceAndFx(device, effect);

	_texture->setDynamic(source_dimensions.x, source_dimensions.y);
	_texture->setAllPixels(source);
	_texture->initTexture();

	// Set the handle on the Rocket texture structure.
	texture_handle = (Rocket::Core::TextureHandle)_texture;
	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void RenderInterfaceDx10 :: ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	Texture* _texture = ((Texture*) texture_handle);
	delete _texture;
}

// Returns the native horizontal texel offset for the renderer.
float RenderInterfaceDx10 :: GetHorizontalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

// Returns the native vertical texel offset for the renderer.
float RenderInterfaceDx10 :: GetVerticalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

void RenderInterfaceDx10::defineInputElementDesc()
{
	// create the input element descriptions
	numElements = 3;
	layoutDesc = new D3D10_INPUT_ELEMENT_DESC[numElements];

	layoutDesc[0].SemanticName = "POSITION";
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	layoutDesc[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	layoutDesc[0].InstanceDataStepRate = 0;

	//use normal instead of color
	layoutDesc[1].SemanticName = "COLOR";
	layoutDesc[1].SemanticIndex = 0;
	layoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layoutDesc[1].InputSlot = 0;
	layoutDesc[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	layoutDesc[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	layoutDesc[1].InstanceDataStepRate = 0;

	//texture
	layoutDesc[2].SemanticName = "TEXCOORD";
	layoutDesc[2].SemanticIndex = 0;
	layoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	layoutDesc[2].InputSlot = 0;
	layoutDesc[2].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	layoutDesc[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	layoutDesc[2].InstanceDataStepRate = 0;
}

void RenderInterfaceDx10::initPipeline()
{
	technique = effect->GetTechniqueByIndex(techNr);
	pass = technique->GetPassByIndex(passNr);
	HR(pass->GetDesc(&PassDesc));

	int korv = 0;

	//layout - input layout description is set in subclass's constructor
	HR(device->CreateInputLayout(layoutDesc, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &inputLayout));
}

void RenderInterfaceDx10::initFxVars()
{
	fxVar_world = effect->GetVariableByName("mat_world")->AsMatrix();
}

void RenderInterfaceDx10::defineBlendState()
{
	D3D10_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend       = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend      = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp        = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha  = D3D10_BLEND_ZERO;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha   = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&blendDesc, &bs_normal));
}
