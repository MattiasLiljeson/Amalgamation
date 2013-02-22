#pragma once
#include <AglMatrix.h>
#include <AglVector3.h>
#include <AglVector2.h>
#include "RendererSceneInfo.h"

// =======================================================================================
//                                      RenderSceneInfoCBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # RenderSceneInfoCBuffer
/// Detailed description.....
/// Created on: 21-1-2013 
///---------------------------------------------------------------------------------------

struct RenderSceneInfoCBuffer
{
	float view[16];
	float viewProj[16];
	float viewProjInv[16];
	float cameraPos[4];
	float cameraForward[4];
	float cameraUp[4];
	float ambientColorAndFogNear[4];
	float fogColorAndFogFar[4];
	float renderTargetSize[2];
	float farPlane;
	float nearPlane;

	void setSceneInfo(const RendererSceneInfo& p_sceneInfo){
		ambientColorAndFogNear[3] = p_sceneInfo.fogNearPlaneClosenessPercentage; // fog near
		fogColorAndFogFar[3] = p_sceneInfo.fogFarPlaneClosenessPercentage;		 // fog far
		setViewProjection(p_sceneInfo.viewProj);
		setViewProjectionInverse(p_sceneInfo.viewProjInv);
		setCameraPos(p_sceneInfo.cameraPos);
		setCameraForward(p_sceneInfo.cameraForward);
		setCameraUp(p_sceneInfo.cameraUp);
		setAmbientColor(p_sceneInfo.ambientColor);
		setFogColor(p_sceneInfo.fogColor);
		setRenderTargetSize(p_sceneInfo.renderTargetDimensions);
		setFarPlane(p_sceneInfo.farPlane);
		setNearPlane(p_sceneInfo.nearPlane);
	}
	void setViewProjection(const AglMatrix& p_viewProj){
		for (unsigned int i = 0; i < 16; i++){
			viewProj[i] = p_viewProj[i];
		}
	}
	void setViewProjectionInverse(const AglMatrix& p_viewProjInv){
		for (unsigned int i = 0; i < 16; i++){
			viewProjInv[i] = p_viewProjInv[i];
		}
	}
	void setCameraPos(const AglVector3& p_pos){
		cameraPos[0] = p_pos[0];
		cameraPos[1] = p_pos[1];
		cameraPos[2] = p_pos[2];
		cameraPos[3] = 1;
	}
	void setCameraForward(const AglVector3& p_forward){
		cameraForward[0] = p_forward[0];
		cameraForward[1] = p_forward[1];
		cameraForward[2] = p_forward[2];
		cameraForward[3] = 0;
	}
	void setCameraUp(const AglVector3& p_up){
		cameraUp[0] = p_up[0];
		cameraUp[1] = p_up[1];
		cameraUp[2] = p_up[2];
		cameraUp[3] = 0;
	}
	void setAmbientColor(const AglVector3& p_ambientColor){
		for (unsigned int i=0;i<3;i++)
			ambientColorAndFogNear[i] = p_ambientColor[i];
	}
	void setFogColor(const AglVector3& p_fogColor){
		for (unsigned int i=0;i<3;i++)
			fogColorAndFogFar[i] = p_fogColor[i];
	}
	void setRenderTargetSize(const AglVector2& p_dimensions){
		renderTargetSize[0] = p_dimensions[0];
		renderTargetSize[1] = p_dimensions[1];
	}
	void setFarPlane(const float& p_far){
		farPlane = p_far;
	}
	void setNearPlane(const float& p_near){
		nearPlane = p_near;
	}
};