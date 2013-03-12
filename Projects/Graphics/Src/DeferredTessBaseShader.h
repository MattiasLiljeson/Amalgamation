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

class DeferredTessBaseShader : public ShaderBase
{
public:
	DeferredTessBaseShader( ShaderVariableContainer p_initData);
	virtual ~DeferredTessBaseShader();

	void apply();
};