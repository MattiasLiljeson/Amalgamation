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

void TW_CALL ParticleSystemDialog::SetSpawnOffset(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnOffset(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetSpawnOffset(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().spawnOffset;
}

void TW_CALL ParticleSystemDialog::SetScreenAlignment(void* clientData)
{

}
void TW_CALL ParticleSystemDialog::SetWorldAlignment(void* clientData)
{

}
void TW_CALL ParticleSystemDialog::SetVelocityDirAlignment(void* clientData)
{

}
void TW_CALL ParticleSystemDialog::SetObserverAlignment(void* clientData)
{

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

void TW_CALL ParticleSystemDialog::SetMaxOpacity(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setMaxOpacity(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetMaxOpacity(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().maxOpacity;
}

void TW_CALL ParticleSystemDialog::SetOffsetOnSphere(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnOffsetType(AglParticleSystemHeader::ONSPHERE);
}
void TW_CALL ParticleSystemDialog::SetOffsetInSphere(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnOffsetType(AglParticleSystemHeader::INSPHERE);
}
void TW_CALL ParticleSystemDialog::SetOffsetOnCircle(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnOffsetType(AglParticleSystemHeader::ONRING);
}
void TW_CALL ParticleSystemDialog::SetOffsetInCircle(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnOffsetType(AglParticleSystemHeader::INRING);
}

void TW_CALL ParticleSystemDialog::SetSpawnOnce(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnType(AglParticleSystemHeader::ONCE);
}
void TW_CALL ParticleSystemDialog::SetSpawnContinously(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
}

void TW_CALL ParticleSystemDialog::SetSpreadPlane(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpreadType(AglParticleSystemHeader::INPLANE);
}
void TW_CALL ParticleSystemDialog::SetSpreadSpace(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setSpreadType(AglParticleSystemHeader::INSPACE);
}

void TW_CALL ParticleSystemDialog::SetParticlesPerSpawn(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setParticlesPerSpawn(*(const unsigned int*)value);
}
void TW_CALL ParticleSystemDialog::GetParticlesPerSpawn(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(unsigned int*)value = ps->GetHeader().particlesPerSpawn;
}

void TW_CALL ParticleSystemDialog::SetParticleSize(const void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->setParticleSize(*(const float*)value);
}
void TW_CALL ParticleSystemDialog::GetParticleSize(void *value, void *clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	*(float*)value = ps->GetHeader().particleSize;
}

void TW_CALL ParticleSystemDialog::Restart(void* clientData)
{
	ParticleSystemDialog* d = (ParticleSystemDialog*)clientData;
	ParticleSystem* ps = Scene::GetInstance()->GetParticleSystem(d->mPSIndex);
	ps->restart();
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


	TwAddVarCB(m_dialog, "Spawn Point", TW_TYPE_DIR3F, SetSpawn, GetSpawn, (void*)this, "key=o");
	TwAddVarCB(m_dialog, "Particle Age", TW_TYPE_FLOAT, SetParticleAge, GetParticleAge, (void*)this, "key=o step=0.01 min=0.0");

	TwAddVarCB(m_dialog, "Spawn Speed", TW_TYPE_FLOAT, SetParticleSpeed, GetParticleSpeed, (void*)this, "key=o step=0.01 min=0.0");
	TwAddVarCB(m_dialog, "Spawn Direction", TW_TYPE_DIR3F, SetParticleDirection, GetParticleDirection, (void*)this, "key=o");

	TwAddVarCB(m_dialog, "Spread", TW_TYPE_FLOAT, SetSpread, GetSpread, (void*)this, "key=o step=0.01 min=0.0 max=1.0");
	TwAddVarCB(m_dialog, "Spawn Frequency", TW_TYPE_FLOAT, SetFrequency, GetFrequency, (void*)this, "key=o step=0.01 min=0.0");
	TwAddVarCB(m_dialog, "Color", TW_TYPE_COLOR4F, SetColor, GetColor, (void*)this, "key=o");

	TwAddVarCB(m_dialog, "Fade-In Stop", TW_TYPE_FLOAT, SetFadeIn, GetFadeIn, (void*)this, "key=o step=0.01");
	TwAddVarCB(m_dialog, "Fade-Out Start", TW_TYPE_FLOAT, SetFadeOut, GetFadeOut, (void*)this, "key=o step=0.01");
	TwAddVarCB(m_dialog, "Spawn Offset", TW_TYPE_FLOAT, SetSpawnOffset, GetSpawnOffset, (void*)this, "key=o step=0.01 min=0.0");
	TwAddVarCB(m_dialog, "Maximum Opacity", TW_TYPE_FLOAT, SetMaxOpacity, GetMaxOpacity, (void*)this, "key=o step=0.01 min=0.0 max=1.0");

	//Offset types
	TwAddButton(m_dialog, "On Sphere", SetOffsetOnSphere, (void*)this, " label='On Sphere' group='Offset Type'");
	TwAddButton(m_dialog, "In Sphere", SetOffsetInSphere, (void*)this, " label='In Sphere' group='Offset Type'");
	TwAddButton(m_dialog, "On Circle", SetOffsetOnCircle, (void*)this, " label='On Circle' group='Offset Type'");
	TwAddButton(m_dialog, "In Circle", SetOffsetInCircle, (void*)this, " label='In Circle' group='Offset Type'");

	//Spawn types
	TwAddButton(m_dialog, "Once", SetSpawnOnce, (void*)this, " label='Once' group='Spawn Type'");
	TwAddButton(m_dialog, "Continuously", SetSpawnContinously, (void*)this, " label='Continuously' group='Spawn Type'");

	//Spread Types
	TwAddButton(m_dialog, "In Plane", SetSpreadPlane, (void*)this, " label='In Plane' group='Spread Type'");
	TwAddButton(m_dialog, "In Space", SetSpreadSpace, (void*)this, " label='In Space' group='Spread Type'");

	//Particle Size
	TwAddVarCB(m_dialog, "Particle Size", TW_TYPE_FLOAT, SetParticleSize, GetParticleSize, (void*)this, "key=o step=0.01 min=0.0");

	TwAddButton(m_dialog, "Restart", Restart, (void*)this, " label='Restart'");
	TwAddButton(m_dialog, "Load Texture", LoadTexture, this, " label='Load Texture'");

	//Particles per spawn
	TwAddVarCB(m_dialog, "Particles Per Spawn", TW_TYPE_UINT32, SetParticlesPerSpawn, GetParticlesPerSpawn, (void*)this, "key=o");

	//Particle Alignment
	//TwDefine("ParticleSystem/'Billboard Alignment' opened=false ");
	TwAddButton(m_dialog, "Screen Alignment", SetScreenAlignment, (void*)this, " label='Screen Alignment' group='Billboard Alignment'");
	TwAddButton(m_dialog, "World Alignment", SetWorldAlignment, (void*)this, " label='World Alignment' group='Billboard Alignment'");
	TwAddButton(m_dialog, "Velocity Alignment", SetVelocityDirAlignment, (void*)this, " label='Velocity Alignment' group='Billboard Alignment'");
	TwAddButton(m_dialog, "Observer Alignment", SetObserverAlignment, (void*)this, " label='Observer Alignment' group='Billboard Alignment'");

	show();
}

static void TW_CALL SetScreenAlignment(void* clientData);
static void TW_CALL SetWorldAlignment(void* clientData);
static void TW_CALL SetVelocityDirAlignment(void* clientData);
static void TW_CALL SetObserverAlignment(void* clientData);