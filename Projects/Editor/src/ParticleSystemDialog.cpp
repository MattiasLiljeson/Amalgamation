#include "ParticleSystemDialog.h"
#include "Scene.h"
#include "TextureManager.h"
#include <AglVector4.h>

void TW_CALL ParticleSystemDialog::SetSpawn(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->SetSpawnPoint(*(const AglVector3*)value);
}
void TW_CALL ParticleSystemDialog::GetSpawn(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(AglVector3*)value = ps->GetHeader().spawnPoint;
}
void TW_CALL ParticleSystemDialog::SetParticleAge(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setParticleAge(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetParticleAge(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().particleAge;
}
void TW_CALL ParticleSystemDialog::SetParticleDirection(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnDirection(*(const AglVector3*)value);
}
void TW_CALL ParticleSystemDialog::GetParticleDirection(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(AglVector3*)value = ps->GetHeader().spawnDirection;
}
void TW_CALL ParticleSystemDialog::SetParticleSpeed(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnSpeed(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetParticleSpeed(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().spawnSpeed;
}
void TW_CALL ParticleSystemDialog::SetColor(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setColor(*(const AglVector4*)value);
}
void TW_CALL ParticleSystemDialog::GetColor(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(AglVector4*)value = ps->getColor();
}

void TW_CALL ParticleSystemDialog::SetFadeIn(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setFadeInStop(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetFadeIn(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->getFadeInStop();
}
void TW_CALL ParticleSystemDialog::SetFadeOut(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setFadeOutStart(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetFadeOut(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->getFadeOutStart();
}

void TW_CALL ParticleSystemDialog::LoadTexture(void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		ps->setTextureIndex(Scene::GetInstance()->AddName(file));
	}
}


void TW_CALL ParticleSystemDialog::SetFrequency(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnFrequency(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetFrequency(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().spawnFrequency;
}

void TW_CALL ParticleSystemDialog::SetSpread(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpread(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetSpread(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().spread;
}

ParticleSystemDialog::ParticleSystemDialog()
{
	hide();
}
ParticleSystemDialog::~ParticleSystemDialog()
{
}
void ParticleSystemDialog::show()
{
	TwDefine(" ParticleSystem visible=true ");
}
void ParticleSystemDialog::hide()
{
	TwDefine(" ParticleSystem visible=false ");
}
void ParticleSystemDialog::setPS(int pIndex)
{
	mPSIndex = pIndex;
	if (m_dialog)
		TwDeleteBar(m_dialog);
	// Create a tweak bar
	m_dialog = TwNewBar("ParticleSystem");
	int barSize[2] = {200, 300};
	TwDefine(" ParticleSystem position='1070 410' ");
	TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);


	TwAddVarCB(m_dialog, "Spawn Point", TW_TYPE_DIR3F, SetSpawn, GetSpawn, (void*)this, "group=Sponge key=o");
	TwAddVarCB(m_dialog, "Particle Age", TW_TYPE_FLOAT, SetParticleAge, GetParticleAge, (void*)this, "group=Sponge key=o step=0.01 min=0.0");

	TwAddVarCB(m_dialog, "Spawn Speed", TW_TYPE_FLOAT, SetParticleSpeed, GetParticleSpeed, (void*)this, "group=Sponge key=o step=0.01 min=0.0");
	TwAddVarCB(m_dialog, "Spawn Direction", TW_TYPE_DIR3F, SetParticleDirection, GetParticleDirection, (void*)this, "group=Sponge key=o");

	TwAddVarCB(m_dialog, "Spread", TW_TYPE_FLOAT, SetSpread, GetSpread, (void*)this, "group=Sponge key=o step=0.01 min=0.0 max=1.0");
	TwAddVarCB(m_dialog, "Spawn Frequency", TW_TYPE_FLOAT, SetFrequency, GetFrequency, (void*)this, "group=Sponge key=o step=0.01 min=0.0");
	TwAddVarCB(m_dialog, "Color", TW_TYPE_COLOR4F, SetColor, GetColor, (void*)this, "group=Sponge key=o");

	TwAddVarCB(m_dialog, "Fade-In Stop", TW_TYPE_FLOAT, SetFadeIn, GetFadeIn, (void*)this, "group=Sponge key=o step=0.01");
	TwAddVarCB(m_dialog, "Fade-Out Start", TW_TYPE_FLOAT, SetFadeOut, GetFadeOut, (void*)this, "group=Sponge key=o step=0.01");

	TwAddButton(m_dialog, "Load Texture", LoadTexture, this, " label='Load Texture'");

	show();
}