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
	delete m_perSystemBuffer;
}

void ParticleShader::apply()
{
	applyStages();
	m_perSystemBuffer->apply();
}

void ParticleShader::unMap()
{
	unApplyStages();
}

Buffer<ParticleCBuffer>* ParticleShader::getPerSystemBuffer(){
	return m_perSystemBuffer;
}