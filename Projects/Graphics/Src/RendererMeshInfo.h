#pragma once

// =======================================================================================
//                             RendererMeshInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Interfacing struct for renderer. Information on what will be rendered.
///        
/// # RendererMeshInfo
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

#include "InstanceVertex.h"

struct RendererMeshInfo
{
	unsigned int p_meshId;
	unsigned int p_textureId;
	InstanceVertex* p_instanceList;
	unsigned int p_numberOfInstances;
};