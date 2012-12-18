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
#pragma once

#include <InstanceData.h>
#include "Buffer.h"
#include "CBuffers.h"
#include "PTVertex.h"
#include "PNTVertex.h"
#include "DIndex.h"
#include "Mesh.h"
// #include "TextureBuffers.h"
// #include "VertexBuffers.h"
// etc?


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

	///-----------------------------------------------------------------------------------
	/// Constructs a vertex buffer.
	/// \param p_vertices
	/// \param p_numberOfElements
	/// \return Buffer<PNTVertex>*
	///-----------------------------------------------------------------------------------
	Buffer<PNTVertex>* createVertexBuffer(PNTVertex* p_vertices,
										  unsigned int p_numberOfElements);

	///-----------------------------------------------------------------------------------
	/// Constructs a index buffer.
	/// \param p_indices
	/// \param p_numberOfElements
	/// \return Buffer<DIndex>*
	///-----------------------------------------------------------------------------------
	Buffer<DIndex>* createIndexBuffer(DIndex* p_indices,
									  unsigned int p_numberOfElements);

	///-----------------------------------------------------------------------------------
	/// This function should create a box mesh only.
	/// \return Box*
	///-----------------------------------------------------------------------------------
	Mesh* createBoxMesh();

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