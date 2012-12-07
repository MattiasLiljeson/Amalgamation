#include "DeferredComposeShader.h"

DeferredComposeShader::DeferredComposeShader( ShaderInitStruct p_initData) 
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
