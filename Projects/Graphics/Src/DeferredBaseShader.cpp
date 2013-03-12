#include "DeferredBaseShader.h"


DeferredBaseShader::DeferredBaseShader( ShaderVariableContainer p_initData) 
									   : ShaderBase(p_initData)
{
}

DeferredBaseShader::~DeferredBaseShader()
{
}

void DeferredBaseShader::apply()
{
	applyStages();
}
