#include "LightShader.h"

LightShader::LightShader( ShaderVariableContainer p_initData ) : ShaderBase(p_initData)
{
	m_shadowSampler = NULL;
}

LightShader::~LightShader()
{

}

void LightShader::apply()
{
	applyStages();
}

