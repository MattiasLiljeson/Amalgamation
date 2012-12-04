// =======================================================================================
//                                      RendererMeshInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Information struct to be filled out by a render system for a mesh to be
/// rendered. This struct is then passed to the renderer itself.
///        
/// # RendererMeshInfo
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

struct RendererMeshInfo
{
public: // all data/formats are temporary, change when necessary
	float position[4];
	float rotation[4];
	int meshId;
};