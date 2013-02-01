#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include "Scene.h"

class MergeDialog
{
private:
	TwBar* m_dialog;
	bool m_importMeshes;
	bool m_importParticleSystems;
	bool m_importMaterials;
	bool m_importAnimations;
private:
	static void TW_CALL LoadAGL(void *clientData);
public:
	MergeDialog();
	~MergeDialog();
	void show();
	void hide();
};

#endif