#ifndef PARTICLESYSTEMDIALOG_H
#define PARTICLESYSTEMDIALOG_H

#include <AntTweakBar.h>
#include "Utility.h"
#include "ParticleSystem.h"

class ParticleSystemDialog
{
private:
	TwBar* m_dialog;
	int mPSIndex;
private:
	static void TW_CALL SetSpawn(const void *value, void *clientData);
	static void TW_CALL GetSpawn(void *value, void *clientData);
	static void TW_CALL SetParticleAge(const void *value, void *clientData);
	static void TW_CALL GetParticleAge(void *value, void *clientData);
	static void TW_CALL SetParticleDirection(const void *value, void *clientData);
	static void TW_CALL GetParticleDirection(void *value, void *clientData);
	static void TW_CALL SetParticleSpeed(const void *value, void *clientData);
	static void TW_CALL GetParticleSpeed(void *value, void *clientData);
	static void TW_CALL SetFrequency(const void *value, void *clientData);
	static void TW_CALL GetFrequency(void *value, void *clientData);
	static void TW_CALL SetSpread(const void *value, void *clientData);
	static void TW_CALL GetSpread(void *value, void *clientData);
	static void TW_CALL SetColor(const void *value, void *clientData);
	static void TW_CALL GetColor(void *value, void *clientData);

	static void TW_CALL SetFadeIn(const void *value, void *clientData);
	static void TW_CALL GetFadeIn(void *value, void *clientData);
	static void TW_CALL SetFadeOut(const void *value, void *clientData);
	static void TW_CALL GetFadeOut(void *value, void *clientData);

	static void TW_CALL SetSpawnOffset(const void *value, void *clientData);
	static void TW_CALL GetSpawnOffset(void *value, void *clientData);

	static void TW_CALL SetMaxOpacity(const void *value, void *clientData);
	static void TW_CALL GetMaxOpacity(void *value, void *clientData);

	static void TW_CALL SetOffsetOnSphere(void* clientData);
	static void TW_CALL SetOffsetInSphere(void* clientData);
	static void TW_CALL SetOffsetOnCircle(void* clientData);
	static void TW_CALL SetOffsetInCircle(void* clientData);

	static void TW_CALL SetSpawnOnce(void* clientData);
	static void TW_CALL SetSpawnContinously(void* clientData);

	static void TW_CALL SetSpreadPlane(void* clientData);
	static void TW_CALL SetSpreadSpace(void* clientData);

	static void TW_CALL SetParticlesPerSpawn(const void *value, void *clientData);
	static void TW_CALL GetParticlesPerSpawn(void *value, void *clientData);

	static void TW_CALL SetParticleSizeX(const void *value, void *clientData);
	static void TW_CALL GetParticleSizeX(void *value, void *clientData);
	static void TW_CALL SetParticleSizeY(const void *value, void *clientData);
	static void TW_CALL GetParticleSizeY(void *value, void *clientData);

	static void TW_CALL SetAlphaBlending(void* clientData);

	//Alignment
	static void TW_CALL SetScreenAlignment(void* clientData);
	static void TW_CALL SetWorldAlignment(void* clientData);
	static void TW_CALL SetVelocityDirAlignment(void* clientData);
	static void TW_CALL SetObserverAlignment(void* clientData);


	static void TW_CALL Restart(void* clientData);
	static void TW_CALL LoadTexture(void *clientData);
public:
	ParticleSystemDialog();
	~ParticleSystemDialog();
	void show();
	void hide();
	void setPS(int pIndex);
};
#endif