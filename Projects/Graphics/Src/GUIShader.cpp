#include "GUIShader.h"

GUIShader::GUIShader( ShaderVariableContainer p_initData ) 
									   : ShaderBase(p_initData)
{
}

GUIShader::~GUIShader()
{
}

void GUIShader::apply()
{
	applyStages();
}
