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
	float worldMat[16];

	float color[4];
	float uvRect[4];
	//float variousFloats[4];	//Containing fadeIn, fadeOut, particleMaxAge, maxOpacity
	float fadeIn;
	float fadeOut;
	float particleMaxAge;
	float maxOpacity;
	int alignment;
	int spawnSpace;
	int particleSpace;
	float pad; //padding

	void setWorldMat( const float p_worldMat[16] )
	{
		memcpy( worldMat, p_worldMat, sizeof(float)*16 );
	}

	void setParticleData(const AglParticleSystemHeader& p_header, const float p_worldMat[16] )
	{
		setWorldMat( p_worldMat );
		setRect(AglVector4(0.0f,0.0f,1.0f,1.0f));
		setColor( p_header.color );
		setFadeIn( p_header.fadeInStop );
		setFadeOut( p_header.fadeOutStart );
		setParticleMaxAge( p_header.particleAge );
		setMaxOpacity( p_header.maxOpacity );
		setAlignment( p_header.alignmentType );
		setSpawnSpace( p_header.spawnSpace );
		setParticleSpace( p_header.particleSpace );
	}

	void setRect(const AglVector4& p_normalizedRect){
		uvRect[0] = p_normalizedRect[0];
		uvRect[1] = p_normalizedRect[1];
		uvRect[2] = p_normalizedRect[2];
		uvRect[3] = p_normalizedRect[3];
	}
	void setColor(const AglVector4& p_color){
		color[0] = p_color[0];
		color[1] = p_color[1];
		color[2] = p_color[2];
		color[3] = p_color[3];
	}
	void setFadeIn(const float& p_fadeIn){
		fadeIn = p_fadeIn;
	}
	void setFadeOut(const float& p_fadeOut){
		fadeOut = p_fadeOut;
	}
	void setParticleMaxAge(const float& p_maxAge){
		particleMaxAge = p_maxAge;
	}
	void setMaxOpacity(const float& p_maxOpacity){
		maxOpacity = p_maxOpacity;
	}
	void setAlignment(const int& p_alignment)
	{
		if (p_alignment == AglParticleSystemHeader::OBSERVER)
			alignment = 0;
		else if (p_alignment == AglParticleSystemHeader::SCREEN)
			alignment = 1;
		else if (p_alignment == AglParticleSystemHeader::WORLD)
			alignment = 2;
		else 
			alignment = 3;
	}
	void setSpawnSpace( int p_relative ) {
		spawnSpace = p_relative;
	}
	void setParticleSpace( int p_space ) {
		particleSpace = p_space;
	}

};