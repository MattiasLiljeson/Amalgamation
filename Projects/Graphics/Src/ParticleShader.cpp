#include "ParticleShader.h"
#include "ShaderInitStruct.h"

ParticleShader::ParticleShader(ShaderInitStruct p_initData) : ShaderBase(p_initData)
{

}

ParticleShader::~ParticleShader()
{

}

void ParticleShader::apply()
{
	applyStages();
}
