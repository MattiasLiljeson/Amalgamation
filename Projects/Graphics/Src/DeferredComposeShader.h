#pragma once

#include "ShaderVariableContainer.h"
#include "ShaderBase.h"
#include "ComposeCBuffer.h"
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
	DeferredComposeShader( Buffer<ComposeCBuffer>* p_composeBuffer, 
		ShaderVariableContainer p_initData);
	virtual ~DeferredComposeShader();

	void apply();
	void setComposeBuffer( const ComposeCBuffer& p_newComposeBuffer );
private:
	Buffer<ComposeCBuffer>* m_composeBuffer;
};