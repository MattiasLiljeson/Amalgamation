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

	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;

	int mTextureIndex;
public:
	ParticleSystem(AglParticleSystem* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~ParticleSystem();

	void SetSpawnPoint(AglVector3 pSpawnPoint)
	{
		mSystem->setSpawnPoint(pSpawnPoint);
	}
	void setParticleAge(float pAge)
	{
		mSystem->setParticleAge(pAge);
	}
	void setSpawnDirection(AglVector3 pDirection)
	{
		mSystem->setSpawnDirection(pDirection);
	}
	void setSpawnSpeed(float pSpeed)
	{
		mSystem->setSpawnSpeed(pSpeed);
	}
	void setSpread(float p_spread)
	{
		mSystem->setSpread(p_spread);
	}
	void setSpawnFrequency(float p_frequency)
	{
		mSystem->setSpawnFrequency(p_frequency);
	}
	AglParticleSystemHeader GetHeader()
	{
		return mSystem->getHeader();
	}
	void setTextureIndex(int p_index)
	{
		mTextureIndex = p_index;
	}
	int getTextureIndex()
	{
		return mTextureIndex;
	}
	void Draw();
};

#endif // PARTICLESYSTEM_H