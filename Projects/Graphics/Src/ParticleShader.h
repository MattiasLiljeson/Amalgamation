#pragma once
#include "ShaderBase.h"
#include "Buffer.h"

struct ShaderInitStruct;
struct ParticleCBuffer;
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
	ParticleShader(ShaderInitStruct p_initData, 
		Buffer<ParticleCBuffer>* p_perSystemBuffer);
	~ParticleShader();
	void apply();
	void unMap();

	Buffer<ParticleCBuffer>* getPerSystemBuffer();
private:
	Buffer<ParticleCBuffer>* m_perSystemBuffer;
};