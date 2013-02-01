#pragma once

#include "ShaderVariableContainer.h"
#include "ShaderBase.h"

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
	DeferredComposeShader( ShaderVariableContainer p_initData);
	virtual ~DeferredComposeShader();

	void apply();
private:

};