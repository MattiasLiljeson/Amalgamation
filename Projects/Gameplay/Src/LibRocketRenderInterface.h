#pragma once

#include <Rocket/Core.h>
#include <vector>

#include <AglMatrix.h>
#include <Buffer.h>
#include <BufferFactory.h>
#include <DIndex.h>
#include <GraphicsWrapper.h>
#include <Mesh.h>
#include <PTVertex.h>
#include <Texture.h>

using namespace std;

// This structure is created for each set of geometry that Rocket compiles.
struct RocketCompiledGeometry
{
	int meshId;
};

class LibRocketRenderInterface : public Rocket::Core::RenderInterface
{

public:
	LibRocketRenderInterface( GraphicsWrapper* p_wrapper );
	virtual ~LibRocketRenderInterface();

	/// Called by Rocket when it wants to render geometry that it does not wish to 
	/// optimize.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices,
		int* indices, int num_indices, Rocket::Core::TextureHandle texture,
		const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for
	/// the foreseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(
		Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices,
		Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
		const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle,
		Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);

	/// Called by Rocket when a texture is required to be built from an 
	///internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
		const byte* source, const Rocket::Core::Vector2i& source_dimensions);

	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

	/// Returns the native horizontal texel offset for the renderer.
	float GetHorizontalTexelOffset();

	/// Returns the native vertical texel offset for the renderer.
	float GetVerticalTexelOffset();

private:
	AglMatrix createWorldMatrix();
	vector<InstanceData> instanceDataVectorFromMatrix( const AglMatrix& p_matrix );

private:
	typedef PNTTBVertex VertexType;	//change this per impl
	BufferFactory* m_factory;
	GraphicsWrapper* m_wrapper;
	AglMatrix m_NDCFrom2dMatrix;

	//States
	// ID3D11RasterizerState* rs_scissorsOn; // state functionality abstracted and moved 
	// ID3D11RasterizerState* rs_scissorsOff; // to graphicswrapper and renderer --Jarl

	//vector<InstanceData> m_NDCFrom2dMatrixVector;
};