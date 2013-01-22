#pragma once

#include "PVertex.h"
#include "DIndex.h"
#include "Buffer.h"
#include "MaterialInfo.h"

// =======================================================================================
//                                      LightMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief		Contains the information to draw a mesh used for lighting.
///        
/// \author		Mattias Liljeson
/// 
/// These meshes represent the range of the light. This is i c/p of the std Mesh but using 
///	PVertex instead of PNTTBVertex. These two could, and should probably be merged.
/// 
/// Created on: 17-1-2013 
///---------------------------------------------------------------------------------------

class LightMesh
{
public:
	LightMesh( Buffer<PVertex>* p_vertexBuffer, Buffer<DIndex>* p_indexBuffer);

	///-----------------------------------------------------------------------------------
	/// The Managers for the mesh will handle the deletion of each entities data.
	/// \return 
	///-----------------------------------------------------------------------------------
	virtual ~LightMesh();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the vertex buffer.
	/// \return Buffer<PNTVertex>*
	///-----------------------------------------------------------------------------------
	Buffer<PVertex>*	getVertexBuffer();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the index buffer.
	/// \return Buffer<DIndex>*
	///-----------------------------------------------------------------------------------
	Buffer<DIndex>*		getIndexBuffer();


	MaterialInfo	getMaterialInfo();					
	void			setTextureId(const MaterialInfo& p_materialInfo);
private:
	Buffer<PVertex>* m_vertexBuffer;
	Buffer<DIndex>* m_indexBuffer;
	MaterialInfo m_materialInfo;
};