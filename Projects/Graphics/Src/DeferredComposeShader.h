#pragma once

#include "ShaderVariableContainer.h"
#include "ShaderBase.h"
#include "SSAOCBuffer.h"
#include "Buffer.h"

// =======================================================================================
//                                 DeferredComposeShader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # DeferredComposeShader
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------


class DeferredComposeShader : public ShaderBase
{
public:
	DeferredComposeShader( Buffer<SSAOBuffer>* p_ssaoBuffer, 
		ShaderVariableContainer p_initData);
	virtual ~DeferredComposeShader();

	void apply();
	void setSSAOBufferData( const SSAOBuffer& p_newSSAOBuffer );
private:
	Buffer<SSAOBuffer>* m_ssaoBuffer;
};