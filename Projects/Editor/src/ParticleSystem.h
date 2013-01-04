// =======================================================================================
//                                      ParticleSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ParticleSystem
/// Detailed description.....
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "AglParticleSystem.h"
#include "Utility.h"

class ParticleSystem
{
private:
	AglParticleSystem* mSystem;
	ID3D11Buffer*	   mVB;
	ID3D11Device*	   mDevice;
	ID3D11DeviceContext* mDeviceContext;
public:
	ParticleSystem(AglParticleSystem* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~ParticleSystem();

	void SetSpawnPoint(AglVector3 pSpawnPoint)
	{
		mSystem->setSpawnPoint(pSpawnPoint);
	}
	AglParticleSystemHeader GetHeader()
	{
		return mSystem->getHeader();
	}
	void Draw();
};

#endif // PARTICLESYSTEM_H