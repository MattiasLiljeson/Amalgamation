#pragma once
#include "ShaderBase.h"
#include "ShadowCBuffer.h"
#include "Buffer.h"

struct AglMatrix;
// =======================================================================================
//                                      ShadowShader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # ShadowShader
/// Detailed description.....
/// Created on: 31-1-2013 
///---------------------------------------------------------------------------------------

class ShadowShader : public ShaderBase
{
public:
	ShadowShader(ShaderVariableContainer p_initData,
		Buffer<ShadowCBuffer>* m_bufferPointer);
	~ShadowShader();

	void apply();

	Buffer<ShadowCBuffer>* getBufferPointer();
	void sendViewProjection(const AglMatrix& p_matrix);
private:
	Buffer<ShadowCBuffer>* m_bufferPointer;
};