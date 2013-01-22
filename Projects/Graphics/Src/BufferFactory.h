#pragma once

#include <InstanceData.h>
#include "Buffer.h"
#include "CBuffers.h"
#include "PTVertex.h"
#include "PNTVertex.h"
#include "PNTTBVertex.h"
#include "DIndex.h"
#include "RenderSceneInfoCBuffer.h"

// #include "TextureBuffers.h"
// #include "VertexBuffers.h"
// etc?

class LightMesh;
class Mesh;

struct LightInstanceData;
struct ParticleCBuffer;

// =======================================================================================
//                                      BufferFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Factory used to construct various buffers
///        
/// # BufferFactory
/// Detailed description.....
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------

class BufferFactory
{
public:
	BufferFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	virtual ~BufferFactory();

	///-----------------------------------------------------------------------------------
	/// Sets up and return a buffer object of type SimpleCBuffer
	/// \return SimpleCBuffer*
	///-----------------------------------------------------------------------------------
	Buffer<SimpleCBuffer>* createSimpleCBuffer();

	Buffer<PTVertex>* createFullScreenQuadBuffer();


	///-----------------------------------------------------------------------------------
	/// Constructs a buffer object of a list of instance transform data
	/// \param p_instanceList 
	/// \param p_numberOfElements
	/// \return Buffer<InstanceVertex>*
	///-----------------------------------------------------------------------------------
	Buffer<InstanceData>* createInstanceBuffer(InstanceData* p_instanceList,
												 unsigned int p_numberOfElements);

	// Duplicate of createInstanceBuffer() but for lights
	Buffer<LightInstanceData>* createLightInstanceBuffer( LightInstanceData* p_instanceList,
		unsigned int p_numberOfElements );

	///-----------------------------------------------------------------------------------
	/// Constructs a vertex buffer of a specified type T.
	/// \param p_vertices
	/// \param p_numberOfElements
	/// \return Buffer<T>*
	///-----------------------------------------------------------------------------------
	template<typename T>
	Buffer<T>* createVertexBuffer(T* p_vertices,
								  unsigned int p_numberOfElements);

	///-----------------------------------------------------------------------------------
	/// Constructs a index buffer.
	/// \param p_indices
	/// \param p_numberOfElements
	/// \return Buffer<DIndex>*
	///-----------------------------------------------------------------------------------
	Buffer<DIndex>* createIndexBuffer(DIndex* p_indices,
									  unsigned int p_numberOfElements);

	Buffer<ParticleCBuffer>* createParticleCBuffer();

	Buffer<RenderSceneInfoCBuffer>*  createRenderSceneInfoCBuffer();


	///-----------------------------------------------------------------------------------
	/// This function should create a box mesh only.
	/// \return Box*
	///-----------------------------------------------------------------------------------
	Mesh* createBoxMesh();

	LightMesh* createLightBoxMesh();

	///-----------------------------------------------------------------------------------
	/// This function should create a sphere mesh only.
	/// \return Sphere*
	///-----------------------------------------------------------------------------------
	Mesh* createSphereMesh();

	///
	Mesh* createMeshFromPNTTBVerticesAndIndices( int p_numVertices, PNTTBVertex* p_vertices,
		int p_numIndices, DIndex* p_indices  );

	Mesh* createMeshFromPTVerticesAndIndices( int p_numVertices, PTVertex* p_vertices,
		int p_numIndices, DIndex* p_indices  );

	///-----------------------------------------------------------------------------------
	/// Constructs an engine Mesh object from raw data.
	/// \param p_vertexBlob
	/// \param p_indexBlob
	/// \param p_numberOfVertices
	/// \param p_numberOfIndices
	/// \return Mesh*
	///-----------------------------------------------------------------------------------
	Mesh* createMeshFromRaw(void* p_vertexBlob, void* p_indexBlob,
							unsigned int p_numberOfVertices,
							unsigned int p_numberOfIndices);

protected:
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

template<typename T>
Buffer<T>* BufferFactory::createVertexBuffer( T* p_vertices, 
											  unsigned int p_numberOfElements )
{		
	Buffer<T>* vertexBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize = sizeof(T);
	vertexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	vertexBufferDesc.NumElements = p_numberOfElements ;
	vertexBufferDesc.Type = BufferConfig::VERTEX_BUFFER;

	vertexBuffer = new Buffer<T>(m_device,m_deviceContext,
		p_vertices,vertexBufferDesc);

	return vertexBuffer;
}
