#include "DeferredComposeShader.h"

DeferredComposeShader::DeferredComposeShader(Buffer<ComposeCBuffer>* p_composeBuffer, 
											 ShaderVariableContainer p_initData)
											: ShaderBase(p_initData)
{
	m_composeBuffer = p_composeBuffer;
}

DeferredComposeShader::~DeferredComposeShader(){
	delete m_composeBuffer;
}

void DeferredComposeShader::apply(){
	applyStages();
	m_composeBuffer->apply();
}

void DeferredComposeShader::setComposeBuffer( const ComposeCBuffer& p_newComposeBuffer ){
	m_composeBuffer->accessBuffer.setData(p_newComposeBuffer);
	m_composeBuffer->update();
}