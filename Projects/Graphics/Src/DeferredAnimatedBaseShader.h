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

class DeferredAnimatedBaseShader : public ShaderBase
{
public:
	DeferredAnimatedBaseShader( ShaderVariableContainer p_initData);
	virtual ~DeferredAnimatedBaseShader();

	void apply();
};