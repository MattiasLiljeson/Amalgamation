#include "ParticleSystemDialog.h"
#include "Scene.h"
#include "TextureManager.h"
#include <AglVector4.h>

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
	if (m_dialog)
		TwDeleteBar(m_dialog);
	// Create a tweak bar
	m_dialog = TwNewBar("ParticleSystem");
	int barSize[2] = {200, 300};
	TwDefine(" ParticleSystem position='1070 410' ");
	TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);
	show();
}