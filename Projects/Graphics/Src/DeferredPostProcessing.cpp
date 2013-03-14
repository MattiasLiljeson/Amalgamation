#include "DeferredPostProcessing.h"


DeferredPostProcessing::DeferredPostProcessing(
	Buffer<PostProcessingBuffer>* p_postProcessingBuffer,
	ShaderVariableContainer p_initData) : ShaderBase(p_initData)
{
	m_postProcessingBuffer = p_postProcessingBuffer;
}

DeferredPostProcessing::~DeferredPostProcessing()
{
	delete m_postProcessingBuffer;
}

void DeferredPostProcessing::apply()
{
	applyStages();
	m_postProcessingBuffer->apply();
}

void DeferredPostProcessing::setPostProcessingBuffer( 
	const PostProcessingBuffer& p_newPosData )
{
	m_postProcessingBuffer->accessBuffer.setData(p_newPosData);
	m_postProcessingBuffer->update();
}