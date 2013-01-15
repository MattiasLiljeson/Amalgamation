#pragma once
#include "ShaderBase.h"

struct ShaderInitStruct;
// =======================================================================================
//                                      ParticleShader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # ParticleShader
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ParticleShader : public ShaderBase
{
public:
	ParticleShader(ShaderInitStruct p_initData);
	~ParticleShader();
	void apply();
};