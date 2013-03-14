#pragma once

#include "ShaderBase.h"
#include "PostProcessingBuffer.h"
#include "Buffer.h"

// =======================================================================================
//                                      DeferredPostProcessing
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # DeferredPostProcessing
/// Detailed description.....
/// Created on: 13-3-2013 
///---------------------------------------------------------------------------------------

class DeferredPostProcessing : public ShaderBase
{
public:
	DeferredPostProcessing( Buffer<PostProcessingBuffer>* p_postProcessingBuffer,
		ShaderVariableContainer p_initData);
	virtual ~DeferredPostProcessing();
	void apply();
	void setPostProcessingBuffer( const PostProcessingBuffer& p_newPosData );
private:
	Buffer<PostProcessingBuffer>* m_postProcessingBuffer;
};