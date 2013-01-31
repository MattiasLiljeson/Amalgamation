#include "ShadowShader.h"
#include <AglMatrix.h>

ShadowShader::ShadowShader(ShaderVariableContainer p_initData, 
						   Buffer<ShadowCBuffer>* m_bufferPointer)
	: ShaderBase(p_initData)
{

}

ShadowShader::~ShadowShader(){

}

void ShadowShader::apply(){
	applyStages();
}

void ShadowShader::sendViewProjection( const AglMatrix& p_matrix ){
	m_bufferPointer->accessBuffer.setViewProjection(p_matrix);
}
