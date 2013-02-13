#include "DeferredAnimatedBaseShader.h"


DeferredAnimatedBaseShader::DeferredAnimatedBaseShader( ShaderVariableContainer p_initData) 
	: ShaderBase(p_initData)
{
}

DeferredAnimatedBaseShader::~DeferredAnimatedBaseShader()
{
}

void DeferredAnimatedBaseShader::apply()
{
	applyStages();
}
