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
#include "AglParticleSystemHeader.h"
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

	float m_scale;
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
	const AglParticleSystemHeader& GetHeader()
	{
		return mSystem->getHeader();
	}
	void setTextureIndex(int p_index)
	{
		mSystem->setTextureNameIndex(p_index);
	}
	int getTextureIndex()
	{
		return mSystem->getHeaderPtr()->textureNameIndex;
	}
	AglVector4 getColor()
	{
		return mSystem->getHeaderPtr()->color;
	}
	void setColor(AglVector4 pColor)
	{
		mSystem->setColor(pColor);
	}

	float getFadeInStop()
	{
		return mSystem->getHeaderPtr()->fadeInStop;
	}
	void setFadeInStop(float pFadeInStop)
	{
		mSystem->setFadeInStop(pFadeInStop);
	}
	float getFadeOutStart()
	{
		return mSystem->getHeaderPtr()->fadeOutStart;
	}
	void setFadeOutStart(float pFadeOutStart)
	{
		mSystem->setFadeOutStart(pFadeOutStart);
	}
	void setSpawnOffset(float pOffset)
	{
		mSystem->setSpawnOffset(pOffset);
	}
	void setMaxOpacity(float pOpacity)
	{
		mSystem->setMaxOpacity(pOpacity);
	}
	void setSpawnOffsetType(AglParticleSystemHeader::AglSpawnOffsetType p_type)
	{
		mSystem->setOffsetType(p_type);
	}
	void restart()
	{
		mSystem->restart();
	}
	void setSpawnType(AglParticleSystemHeader::AglSpawnType p_type)
	{
		mSystem->setSpawnType(p_type);
	}
	void setParticlesPerSpawn(unsigned int p_particlesPerSpawn)
	{
		mSystem->setParticlesPerSpawn(p_particlesPerSpawn);
	}
	void setSpreadType(AglParticleSystemHeader::AglSpreadType p_type)
	{
		mSystem->setSpreadType(p_type);
	}
	void setParticleSizeX(float p_size)
	{
		mSystem->setParticleSize(AglVector2(p_size, mSystem->getHeaderPtr()->particleSize.y));
	}
	void setParticleSizeY(float p_size)
	{
		mSystem->setParticleSize(AglVector2(mSystem->getHeaderPtr()->particleSize.x, p_size));
	}
	AglParticleSystemHeader::AglAlignmentType getAlignment()
	{
		return mSystem->getHeaderPtr()->alignmentType;
	}
	void setAlignmentType(AglParticleSystemHeader::AglAlignmentType pType)
	{
		mSystem->setAlignmentType(pType);
	}
	void setSpawnAngularVelocity(float p_angularVelocity)
	{
		mSystem->setSpawnAngularVelocity(p_angularVelocity);
	}
	float getCameraDistance(AglVector3 p_cameraPos, AglVector3 p_cameraDir)
	{
		return mSystem->getCameraDistance(p_cameraPos, p_cameraDir);
	}
	AglParticleSystem* getParticleSystem()
	{
		return mSystem;
	}
	float getScale()
	{
		return m_scale;
	}
	void Draw(float p_scale);
};

#endif // PARTICLESYSTEM_H