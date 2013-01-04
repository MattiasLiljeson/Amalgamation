#include "ParticleSystem.h"
#include "Globals.h"
#include "SphereMesh.h"

ParticleSystem::ParticleSystem(AglParticleSystem* pSystem)
{
	mSystem = pSystem;
}

ParticleSystem::~ParticleSystem()
{

}
void ParticleSystem::Draw()
{
	AglMatrix psWorld = AglMatrix::createTranslationMatrix(mSystem->getHeader().spawnPoint);
	psWorld[0] = 0.1f;
	psWorld[5] = 0.1f;
	psWorld[10] = 0.1f;
	SPHEREMESH->Draw(psWorld, AglVector3(1, 0.5f, 0));
	psWorld[0] = 0.01f;
	psWorld[5] = 0.01f;
	psWorld[10] = 0.01f;
	SPHEREMESH->Draw(psWorld, AglVector3(0, 0.0f, 0));
}

