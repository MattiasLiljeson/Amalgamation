#include "RocketShader.h"

RocketShader::RocketShader( ShaderInitStruct p_initData, 
									   Buffer<SimpleCBuffer>* p_cBuffer ) 
									   : ShaderBase(p_initData)
{
	m_perFrameBuffer = p_cBuffer;
}

RocketShader::~RocketShader()
{
	delete m_perFrameBuffer;
}

void RocketShader::apply()
{
	m_perFrameBuffer->apply();
	applyStages();
}

Buffer<SimpleCBuffer>* RocketShader::getPerFrameBufferPtr()
{
	return m_perFrameBuffer;
}
