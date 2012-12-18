#ifndef RENDERINTERFACEDX10_H
#define RENDERINTERFACEDX10_H

//#include <d3d10.h>
//#include <d3dx10.h>
#include <vector>

#include <Rocket/Core.h>

#include "BufferFactory.h"
#include "Mesh.h"
//#include "Utils.h"
//#include "VertexTexture.h"
#include "Texture.h"
#include "PTVertex.h"
#include <Buffer.h>
#include <DIndex.h>
#include "GraphicsWrapper.h"
#include <AglMatrix.h>

using namespace std;

// The internal format of the vertex we use for rendering Rocket geometry. 
// We could optimise space by having a second
// untextured vertex for use when rendering coloured borders and backgrounds.
//struct VertexRocket
//{
//	D3DXVECTOR3 pos;
//	D3DXCOLOR	color;
//	D3DXVECTOR2 texCoord;
//};

// This structure is created for each set of geometry that Rocket compiles.
// It stores the vertex and index buffers and
// the texture associated with the geometry, if one was specified.
struct RocketDx10CompiledGeometry
{
	//vector<VertexRocket> vertices;
	//vector<PTNVertex> vertices;
	Buffer<PTVertex> vertices;
	DWORD num_vertices;

	//vector<unsigned int> indices;
	Buffer<DIndex> indices;
	DWORD num_primitives;

	Mesh* mesh;
	Texture* texture;

	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
};

class RenderInterfaceDx10 : public Rocket::Core::RenderInterface
{
private:
	typedef PTVertex VertexType;	//change this per impl
	BufferFactory* m_factory;
	GraphicsWrapper* m_wrapper;

public:
	RenderInterfaceDx10( BufferFactory* p_factory, GraphicsWrapper* p_wrapper );
	virtual ~RenderInterfaceDx10();

	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

	/// Returns the native horizontal texel offset for the renderer.
	float GetHorizontalTexelOffset();
	/// Returns the native vertical texel offset for the renderer.
	float GetVerticalTexelOffset();

	//==============================================
	//Added for dx10
	//==============================================
	void defineInputElementDesc();
	void defineBlendState();

	virtual void initPipeline();
	virtual void initFxVars();
};

#endif //RENDERINTERFACEDX10_H