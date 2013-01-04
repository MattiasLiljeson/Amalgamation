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

	show();
}