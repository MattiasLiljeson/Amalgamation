#pragma once
#include <AglVector3.h>
#include <AglVector4.h>
#include <AglMatrix.h>
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
	float view[16];
	float proj[16];
	float eyePos[4];
	float color[4];
	float forward[4];
	float up[4];
	float variousFloats[4];	//Containing fadeIn, fadeOut, particleMaxAge, maxOpacity
	float alignment[4];

	void setViewMatrix(const AglMatrix& p_view){
		for (unsigned int i = 0; i < 16; i++){
			view[i] = p_view[i];
		}
	}
	void setProjMatrix(const AglMatrix& p_proj){
		for (unsigned int i = 0; i < 16; i++){
			proj[i] = p_proj[i];
		}
	}
	void setColor(const AglVector4& p_color){
		color[0] = p_color[0];
		color[1] = p_color[1];
		color[2] = p_color[2];
		color[3] = p_color[3];
	}
	void setEyePos(const AglVector3& p_pos){
		eyePos[0] = p_pos[0];
		eyePos[1] = p_pos[1];
		eyePos[2] = p_pos[2];
		eyePos[3] = 1;
	}
	void setCameraForward(const AglVector3& p_forward){
		forward[0] = p_forward[0];
		forward[1] = p_forward[1];
		forward[2] = p_forward[2];
		forward[3] = 0;
	}
	void setCameraUp(const AglVector3& p_up){
		up[0] = p_up[0];
		up[1] = p_up[1];
		up[2] = p_up[2];
		up[3] = 0;
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
	void setAlignment(const float& p_alignment){
		alignment[0] = p_alignment;
		alignment[1] = 0;
		alignment[2] = 0;
		alignment[3] = 0;
	}
};