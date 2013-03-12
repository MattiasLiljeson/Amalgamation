#include "DeferredComposeShader.h"

DeferredComposeShader::DeferredComposeShader(Buffer<SSAOBuffer>* p_ssaoBuffer, 
											 ShaderVariableContainer p_initData)
											: ShaderBase(p_initData)
{
	m_ssaoBuffer = p_ssaoBuffer;
}

DeferredComposeShader::~DeferredComposeShader(){
	delete m_ssaoBuffer;
}

void DeferredComposeShader::apply(){
	applyStages();
	m_ssaoBuffer->apply();
}

void DeferredComposeShader::setSSAOBufferData( const SSAOBuffer& p_newSSAOBuffer ){
	m_ssaoBuffer->accessBuffer.setData(p_newSSAOBuffer);
	m_ssaoBuffer->update();
}