#include "DeferredBaseShader.h"


DeferredBaseShader::DeferredBaseShader( ShaderInitStruct p_initData, 
									   Buffer<SimpleCBuffer>* p_cBuffer ) 
									   : ShaderBase(p_initData)
{
	m_perFrameBuffer = p_cBuffer;
}

DeferredBaseShader::~DeferredBaseShader()
{
	delete m_perFrameBuffer;
}

void DeferredBaseShader::apply()
{
	m_perFrameBuffer->apply();
	applyStages();
}

Buffer<SimpleCBuffer>* DeferredBaseShader::getPerFrameBufferPtr()
{
	return m_perFrameBuffer;
}
