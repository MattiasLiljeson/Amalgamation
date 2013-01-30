#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <AntTweakBar.h>
#include "Utility.h"

class MaterialDialog
{
private:
	TwBar* m_dialog;
	AglVector3 m_diffuse;
	//int m_material;
	AglMaterial* m_material;
private:
	//Callbacks
	static void TW_CALL LoadDiffuse(void *clientData);
	static void TW_CALL LoadSpecular(void *clientData);
	static void TW_CALL LoadGlow(void *clientData);
	static void TW_CALL LoadNormal(void *clientData);
	static void TW_CALL LoadDisplacement(void *clientData);
	static void TW_CALL LoadGradient(void *clientData);
	static void TW_CALL SetName(const void *value, void *clientData);
	static void TW_CALL GetName(void *value, void *clientData);
	static void TW_CALL AddLayer(void *clientData);
	static void TW_CALL Delete(void *clientData);
public:
	MaterialDialog();
	~MaterialDialog();
	void show();
	void hide();
	void setMaterial(int pIndex);
};
#endif