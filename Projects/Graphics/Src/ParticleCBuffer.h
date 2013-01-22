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
	float color[4];
	float variousFloats[4];	//Containing fadeIn, fadeOut, particleMaxAge, maxOpacity
	float alignment[4];

	void setParticleData(const AglParticleSystemHeader& p_header){
		setColor(p_header.color);
		setFadeIn(p_header.fadeInStop);
		setFadeOut(p_header.fadeOutStart);
		setParticleMaxAge(p_header.particleAge);
		setMaxOpacity(p_header.maxOpacity);
		setAlignment(p_header.alignmentType);
	}
	void setColor(const AglVector4& p_color){
		color[0] = p_color[0];
		color[1] = p_color[1];
		color[2] = p_color[2];
		color[3] = p_color[3];
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