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

class DeferredBaseShader : public ShaderBase
{
public:
	DeferredBaseShader( ShaderInitStruct p_initData, Buffer<SimpleCBuffer>* p_cBuffer );
	virtual ~DeferredBaseShader();

	Buffer<SimpleCBuffer>* getPerFrameBufferPtr();

	void apply();
private:
	Buffer<SimpleCBuffer>* m_perFrameBuffer;
};