#include "ParticleSystem.h"
#include "Globals.h"
#include "SphereMesh.h"

ParticleSystem::ParticleSystem(AglParticleSystem* pSystem, ID3D11Device* pDevice)
{
	mSystem = pSystem;
	mDevice = pDevice;
	mVB = NULL;
}

ParticleSystem::~ParticleSystem()
{
	if (mVB)
		mVB->Release();
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


	vector<void*> particles = mSystem->getParticles();

	//Update vertex buffer
	if (particles.size() > 0)
	{
		if (mVB)
			mVB->Release();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(AglStandardParticle) * particles.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &particles[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		mDevice->CreateBuffer(&bd, &vertexData, &mVB);
	}
}

