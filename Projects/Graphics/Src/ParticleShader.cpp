#include "ParticleShader.h"
#include "ShaderInitStruct.h"
#include "ParticleCBuffer.h"

ParticleShader::ParticleShader(ShaderInitStruct p_initData, 
							   Buffer<ParticleCBuffer>* p_perSystemBuffer)
							   : ShaderBase(p_initData)
{
	m_perSystemBuffer = p_perSystemBuffer;
}

ParticleShader::~ParticleShader()
{

}

void ParticleShader::apply()
{
	applyStages();
}

Buffer<ParticleCBuffer>* ParticleShader::getPerSystemBuffer(){
	return m_perSystemBuffer;
}