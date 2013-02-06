#pragma once
#include "ShaderVariableContainer.h"
#include "ShaderBase.h"
#include "Buffer.h"
#include "CBuffers.h"

// =======================================================================================
//                                      Shader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Shader
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------

class DeferredBaseShader : public ShaderBase
{
public:
	DeferredBaseShader( ShaderVariableContainer p_initData);
	virtual ~DeferredBaseShader();

	void apply();
};