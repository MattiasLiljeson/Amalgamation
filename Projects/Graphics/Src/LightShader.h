#pragma once

#include "ShaderBase.h"

// =======================================================================================
//                                      LightShader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LightShader
/// Detailed description.....
/// Created on: 7-2-2013 
///---------------------------------------------------------------------------------------

class LightShader : public ShaderBase
{
public:
	LightShader( ShaderVariableContainer p_initData );
	LightShader( ShaderVariableContainer p_initData, ID3D11SamplerState* p_shadowSampler);
	virtual ~LightShader();

	void apply();
private:
	ID3D11SamplerState* m_shadowSampler;
};