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

class DeferredTessAnimatedBaseShader : public ShaderBase
{
public:
	DeferredTessAnimatedBaseShader( ShaderVariableContainer p_initData);
	virtual ~DeferredTessAnimatedBaseShader();

	void apply();
};