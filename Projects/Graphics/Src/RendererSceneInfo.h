#pragma once
#include <AglVector2.h>
#include <AglVector3.h>
#include <AglMatrix.h>

// =======================================================================================
//                                      RendererSceneInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # RendererSceneInfo
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------

struct RendererSceneInfo
{
	AglMatrix view;
	AglMatrix viewProj;
	AglVector3 cameraPos;
	AglVector3 cameraForward;
	AglVector3 cameraUp;
	AglMatrix viewProjInv;
	AglVector2 renderTargetDimensions;
	float farPlane;
	float nearPlane;
};