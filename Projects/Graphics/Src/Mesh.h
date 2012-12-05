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
#include "PTNVertex.h"
#include "DIndex.h"
#include "Buffer.h"


class Mesh
{
public:
	Mesh( Buffer<PTNVertex>* p_mesh , Buffer<DIndex>* p_indices);
	///-----------------------------------------------------------------------------------
	/// The Managers for the mesh will handle the deletion of each entities data.
	/// \returns 
	///-----------------------------------------------------------------------------------
	virtual ~Mesh();

	Buffer<PTNVertex>*	getMesh();
	Buffer<DIndex>*		getIndicies();

	unsigned int	getInstanceId(unsigned int p_idx);
	void			addInstanceId(unsigned int p_instanceId);
	void			clearInstanceList(unsigned int p_idx);
private:
	Buffer<PTNVertex>* m_mesh;
	Buffer<DIndex>* m_indices;
	UniqueIndexList<unsigned int> m_instanceList;
	// texture id / material id
};