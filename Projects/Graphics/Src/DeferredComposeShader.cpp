#include "DeferredComposeShader.h"

DeferredComposeShader::DeferredComposeShader( ShaderVariableContainer p_initData) 
											: ShaderBase(p_initData)
{

}

DeferredComposeShader::~DeferredComposeShader()
{

}

void DeferredComposeShader::apply()
{
	applyStages();
}
