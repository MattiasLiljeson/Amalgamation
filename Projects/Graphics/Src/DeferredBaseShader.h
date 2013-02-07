#pragma once
#include "ShaderBase.h"

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