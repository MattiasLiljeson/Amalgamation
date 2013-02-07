#include "LightShader.h"

LightShader::LightShader( ShaderVariableContainer p_initData ) : ShaderBase(p_initData)
{
	m_shadowSampler = NULL;
}

LightShader::LightShader(ShaderVariableContainer p_initData, 
						 ID3D11SamplerState* p_shadowSampler) : ShaderBase(p_initData) 
{
	m_shadowSampler = p_shadowSampler;
}


LightShader::~LightShader()
{

}

void LightShader::apply()
{
	applyStages();
	if(m_shadowSampler)
		applyCustomSamplerState(m_shadowSampler,1);
}

