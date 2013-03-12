#include "ShadowShader.h"
#include <AglMatrix.h>

ShadowShader::ShadowShader(ShaderVariableContainer p_initData, 
						   Buffer<ShadowCBuffer>* p_bufferPointer)
	: ShaderBase(p_initData)
{
	m_bufferPointer = p_bufferPointer;
}

ShadowShader::~ShadowShader(){
	delete m_bufferPointer;
}

void ShadowShader::apply(){
	m_bufferPointer->update();
	m_bufferPointer->apply();
	applyStages();
}

void ShadowShader::sendViewProjection( const AglMatrix& p_matrix ){
	m_bufferPointer->accessBuffer.setViewProjection(p_matrix);
}

void ShadowShader::sendViewProjections( AglMatrix* p_matrix ){
	m_bufferPointer->accessBuffer.setViewProjections(p_matrix);
}
