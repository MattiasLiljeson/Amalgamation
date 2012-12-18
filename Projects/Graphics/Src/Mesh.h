// =======================================================================================
//                                      MEsh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the information to draw a mesh
///        
/// # Mesh
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <UniqueIndexList.h>
#include "PNTVertex.h"
#include "DIndex.h"
#include "Buffer.h"


class Mesh
{
public:
	Mesh( Buffer<PNTVertex>* p_vertexBuffer, Buffer<DIndex>* p_indexBuffer);
	///-----------------------------------------------------------------------------------
	/// The Managers for the mesh will handle the deletion of each entities data.
	/// \return 
	///-----------------------------------------------------------------------------------
	virtual ~Mesh();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the vertex buffer.
	/// \return Buffer<PNTVertex>*
	///-----------------------------------------------------------------------------------
	Buffer<PNTVertex>*	getVertexBuffer();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the index buffer.
	/// \return Buffer<DIndex>*
	///-----------------------------------------------------------------------------------
	Buffer<DIndex>*		getIndexBuffer();


	unsigned int	getTextureId();					// Replace these with
	void			setTextureId(unsigned int p_id);// material functionality later.
private:
	Buffer<PNTVertex>* m_vertexBuffer;
	Buffer<DIndex>* m_indexBuffer;
	unsigned int m_textureId; // replace for material id/material id list
};