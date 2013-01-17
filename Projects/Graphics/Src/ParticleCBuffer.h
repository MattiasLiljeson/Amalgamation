#pragma once
#include <AglVector3.h>
#include <AglVector4.h>
#include <AglMatrix.h>
#include <AglParticleSystemHeader.h>
#include "RendererSceneInfo.h"
// =======================================================================================
//                                      ParticleCBuffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Stores all the values used by the particle constant buffer.
///        
/// # ParticleCBuffer
/// Detailed description.....
/// Created on: 16-1-2013 
///---------------------------------------------------------------------------------------

struct ParticleCBuffer
{
	float viewProj[16];
	float color[4];
	float cameraPos[4];
	float cameraForward[4];
	float cameraUp[4];
	float variousFloats[4];	//Containing fadeIn, fadeOut, particleMaxAge, maxOpacity
	float alignment[4];

	void setSceneInfo(const RendererSceneInfo& p_sceneInfo){
		setViewProjection(p_sceneInfo.viewProj);
		setCameraPos(p_sceneInfo.cameraPos);
		setCameraForward(p_sceneInfo.cameraForward);
		setCameraUp(p_sceneInfo.cameraUp);
	}
	void setParticleData(const AglParticleSystemHeader& p_header){
		setColor(p_header.color);
		setFadeIn(p_header.fadeInStop);
		setFadeOut(p_header.fadeOutStart);
		setParticleMaxAge(p_header.particleAge);
		setMaxOpacity(p_header.maxOpacity);
		setAlignment(p_header.alignmentType);
	}
	void setViewProjection(const AglMatrix& p_viewProj){
		for (unsigned int i = 0; i < 16; i++){
			viewProj[i] = p_viewProj[i];
		}
	}
	void setColor(const AglVector4& p_color){
		color[0] = p_color[0];
		color[1] = p_color[1];
		color[2] = p_color[2];
		color[3] = p_color[3];
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
	void setFadeIn(const float& p_fadeIn){
		variousFloats[0] = p_fadeIn;
	}
	void setFadeOut(const float& p_fadeOut){
		variousFloats[1] = p_fadeOut;
	}
	void setParticleMaxAge(const float& p_maxAge){
		variousFloats[2] = p_maxAge;
	}
	void setMaxOpacity(const float& p_maxOpacity){
		variousFloats[3] = p_maxOpacity;
	}
	void setAlignment(const int& p_alignment){
		alignment[0] = static_cast<int>(p_alignment);
		alignment[1] = 0;
		alignment[2] = 0;
		alignment[3] = 0;
	}
};