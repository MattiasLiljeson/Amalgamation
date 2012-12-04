// =======================================================================================
//                                      Box
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the information to draw a box
///        
/// # Box
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include "PTNVertex.h"
#include "DIndex.h"
#include "Buffer.h"

class Box
{
public:
	Box( Buffer<PTNVertex>* p_mesh , Buffer<DIndex>* p_indices);
	///-----------------------------------------------------------------------------------
	/// The Managers for the mesh will handle the deletion of each entities data.
	/// \returns 
	///-----------------------------------------------------------------------------------
	virtual ~Box();

	Buffer<PTNVertex>*	getMesh();
	Buffer<DIndex>*		getIndicies();
private:
	Buffer<PTNVertex>* m_mesh;
	Buffer<DIndex>* m_indices;
};