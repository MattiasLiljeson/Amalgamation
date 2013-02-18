#include "DeferredTessBaseShader.h"


DeferredTessBaseShader::DeferredTessBaseShader( ShaderVariableContainer p_initData) 
	: ShaderBase(p_initData)
{
}

DeferredTessBaseShader::~DeferredTessBaseShader()
{
}

void DeferredTessBaseShader::apply()
{
	applyStages();
}
