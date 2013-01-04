#include "GUIShader.h"

GUIShader::GUIShader( ShaderInitStruct p_initData, 
									   Buffer<SimpleCBuffer>* p_cBuffer ) 
									   : ShaderBase(p_initData)
{
	m_perFrameBuffer = p_cBuffer;
}

GUIShader::~GUIShader()
{
	delete m_perFrameBuffer;
}

void GUIShader::apply()
{
	m_perFrameBuffer->apply();
	applyStages();
}

Buffer<SimpleCBuffer>* GUIShader::getPerFrameBufferPtr()
{
	return m_perFrameBuffer;
}
