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
public:
	ParticleSystemDialog();
	~ParticleSystemDialog();
	void show();
	void hide();
	void setPS(int pIndex);
};
#endif