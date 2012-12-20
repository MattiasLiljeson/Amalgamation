#pragma once

#include "ShaderInitStruct.h"
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
	DeferredComposeShader( ShaderInitStruct p_initData);
	virtual ~DeferredComposeShader();

	void apply();
private:

};