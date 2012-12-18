// =======================================================================================
//                                      PNTTBVertex
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A basic struct containing Position Normal Texture Tangent and Binormal
///        
/// # PNTTBVertex
/// Detailed description.....
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

struct PNTTBVertex
{
	float pos[3];
	float normal[3];	
	float texCoord[2];
	float tangent[3];
	float binormal[3];
};