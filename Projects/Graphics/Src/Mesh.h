#pragma once

#include "PNTVertex.h"
#include "PNTTBVertex.h"
#include "DIndex.h"
#include "Buffer.h"
#include "MaterialInfo.h"

// =======================================================================================
//                                      Mesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the information to draw a mesh
///        
/// # Mesh
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------

class Mesh
{
public:
	Mesh( Buffer<PNTTBVertex>* p_vertexBuffer, Buffer<DIndex>* p_indexBuffer);

	///-----------------------------------------------------------------------------------
	/// The Managers for the mesh will handle the deletion of each entities data.
	/// \return 
	///-----------------------------------------------------------------------------------
	virtual ~Mesh();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the vertex buffer.
	/// \return Buffer<PNTVertex>*
	///-----------------------------------------------------------------------------------
	Buffer<PNTTBVertex>*	getVertexBuffer();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the index buffer.
	/// \return Buffer<DIndex>*
	///-----------------------------------------------------------------------------------
	Buffer<DIndex>*		getIndexBuffer();


	MaterialInfo	getMaterialInfo();					
	void			setMaterial(const MaterialInfo& p_materialInfo);
private:
	Buffer<PNTTBVertex>* m_vertexBuffer;
	Buffer<DIndex>* m_indexBuffer;
	MaterialInfo m_materialInfo;
};