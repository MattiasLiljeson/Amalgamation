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
#pragma once

#include <string>
#include <comdef.h>
#include "ShaderInitStruct.h"
#include "ShaderBase.h"
#include "D3DException.h"
#include "ToString.h"
#include "Buffer.h"
#include "CBuffers.h"

using namespace std;

class DeferredComposeShader : public ShaderBase
{
public:
	DeferredComposeShader( ShaderInitStruct p_initData);
	virtual ~DeferredComposeShader();

	void apply();
private:

};