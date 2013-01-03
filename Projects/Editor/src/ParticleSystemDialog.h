#ifndef PARTICLESYSTEMDIALOG_H
#define PARTICLESYSTEMDIALOG_H

#include <AntTweakBar.h>
#include "Utility.h"

class ParticleSystemDialog
{
private:
	TwBar* m_dialog;
public:
	ParticleSystemDialog();
	~ParticleSystemDialog();
	void show();
	void hide();
	void setPS(int pIndex);
};
#endif