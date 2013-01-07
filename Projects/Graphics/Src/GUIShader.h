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
#include "ShaderInitStruct.h"
#include "ShaderBase.h"
#include "D3DException.h"
#include "ToString.h"
#include "Buffer.h"
#include "CBuffers.h"

using namespace std;

class GUIShader : public ShaderBase
{
public:
	GUIShader( ShaderInitStruct p_initData, Buffer<SimpleCBuffer>* p_cBuffer );
	virtual ~GUIShader();

	Buffer<SimpleCBuffer>* getPerFrameBufferPtr();

	void apply();
private:
	Buffer<SimpleCBuffer>* m_perFrameBuffer;
};