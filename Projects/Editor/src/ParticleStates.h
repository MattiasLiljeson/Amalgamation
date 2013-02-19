// =======================================================================================
//                                      ParticleStates
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ParticleStates
/// Detailed description.....
/// Created on: 19-2-2013 
///---------------------------------------------------------------------------------------
#ifndef PARTICLESTATES_H
#define PARTICLESTATES_H

#include "Utility.h"

class ParticleStates
{
public:
	static ID3D11BlendState* AlphaBlending;
	static ID3D11BlendState* AdditiveBlending;
	static ID3D11BlendState* MultiplyBlending;
	static ID3D11DepthStencilState* CullZ;
	static ID3D11DepthStencilState* CullNone;
	static void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

};

#endif // PARTICLESTATES_H