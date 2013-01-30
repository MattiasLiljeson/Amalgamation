#ifndef SCENEDIALOG_H
#define SCENEDIALOG_H

#include <AntTweakBar.h>
#include "Mesh.h"
#include "Scene.h"
#include "AGLLoader.h"
#include "MeshDialog.h"
#include "MaterialDialog.h"
#include "ParticleSystemDialog.h"
#include "MergeDialog.h"

class SceneDialog
{
private:
	static SceneDialog* sInstance;
private:
	TwBar* m_dialog;

	//Spawned Dialog
	MeshDialog* m_meshDialog;
	MaterialDialog* m_materialDialog;
	ParticleSystemDialog* m_particleSystemDialog;
	MergeDialog*			m_mergeDialog;
private:
	//Private Functions
	SceneDialog();
	virtual ~SceneDialog();
private:
	//Callbacks
	static void TW_CALL OpenMeshDialog(void *clientData);
	static void TW_CALL OpenParticleSystemDialog(void *clientData);
	static void TW_CALL OpenMaterialDialog(void *clientData);
	static void TW_CALL OpenMergeDialog(void *clientData);
	static void TW_CALL LoadAGL(void *clientData);
	static void TW_CALL SaveAGL(void *clientData);
	static void TW_CALL AddMaterial(void *clientData);
	static void TW_CALL AddPE(void* clientData);

	static void TW_CALL SetCOSystem(void *clientData);
public:
	//Static functions
	static SceneDialog* GetInstance();
	static void Release();

	void ClonePE(AglParticleSystemHeader pHeader);
public:
	void SetCurrentMesh(int pIndex);
	void SetCurrentParticleSystem(int pIndex);
	void SetCurrentMaterial(int pIndex);
};

#endif