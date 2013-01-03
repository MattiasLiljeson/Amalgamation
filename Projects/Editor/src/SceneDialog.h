#ifndef SCENEDIALOG_H
#define SCENEDIALOG_H

#include <AntTweakBar.h>
#include "Mesh.h"
#include "Scene.h"
#include "AGLLoader.h"
#include "MeshDialog.h"
#include "MaterialDialog.h"

class SceneDialog
{
private:
	static SceneDialog* sInstance;
private:
	TwBar* m_dialog;

	//Spawned Dialog
	MeshDialog* m_meshDialog;
	MaterialDialog* m_materialDialog;
private:
	//Private Functions
	SceneDialog();
	virtual ~SceneDialog();
private:
	//Callbacks
	static void TW_CALL OpenMeshDialog(void *clientData);
	static void TW_CALL OpenMaterialDialog(void *clientData);
	static void TW_CALL LoadAGL(void *clientData);
	static void TW_CALL SaveAGL(void *clientData);
	static void TW_CALL AddMaterial(void *clientData);
	static void TW_CALL AddPE(void* clientData);

	static void TW_CALL SetCOSystem(void *clientData);
public:
	//Static functions
	static SceneDialog* GetInstance();
	static void Release();
public:
	void SetCurrentMesh(int pIndex);
	void SetCurrentMaterial(int pIndex);
};

#endif