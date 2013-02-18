#include "DeferredTessAnimatedBaseShader.h"


DeferredTessAnimatedBaseShader::DeferredTessAnimatedBaseShader( ShaderVariableContainer p_initData) 
	: ShaderBase(p_initData)
{
}

DeferredTessAnimatedBaseShader::~DeferredTessAnimatedBaseShader()
{
}

void DeferredTessAnimatedBaseShader::apply()
{
	applyStages();
}
