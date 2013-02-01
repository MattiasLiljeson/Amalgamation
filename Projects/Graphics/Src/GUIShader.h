// =======================================================================================
//                                     GUI Shader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	GUIShader class based on deferred base shader 
///        
/// # Shader
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <comdef.h>
#include "ShaderVariableContainer.h"
#include "ShaderBase.h"
#include "D3DException.h"
#include "ToString.h"
#include "Buffer.h"
#include "CBuffers.h"

using namespace std;

class GUIShader : public ShaderBase
{
public:
	GUIShader( ShaderVariableContainer p_initData);
	virtual ~GUIShader();

	void apply();
};