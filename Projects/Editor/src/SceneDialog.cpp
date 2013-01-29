#include "SceneDialog.h"
#include "Globals.h"

SceneDialog* SceneDialog::sInstance = NULL;

void TW_CALL SceneDialog::OpenMeshDialog(void *clientData)
{
	int index = (int)clientData;
	SceneDialog::GetInstance()->SetCurrentMesh(index);
}
void TW_CALL SceneDialog::OpenParticleSystemDialog(void *clientData)
{
	int index = (int)clientData;
	SceneDialog::GetInstance()->SetCurrentParticleSystem(index);
}
void TW_CALL SceneDialog::OpenMaterialDialog(void *clientData)
{
	int index = (int)clientData;
	SceneDialog::GetInstance()->SetCurrentMaterial(index);
}
void TW_CALL SceneDialog::SetCOSystem(void *clientData)
{
	int index = (int)clientData;
	if (index == 0)
	{
		//FLIP X
		AglMatrix mat(-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Scene::GetInstance()->Transform(mat);
	}
	else if (index == 1)
	{
		//FLIP Y
		AglMatrix mat(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Scene::GetInstance()->Transform(mat);
	}
	else if (index == 2)
	{
		//FLIP Z
		AglMatrix mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
		Scene::GetInstance()->Transform(mat);
	}
	else if (index == 3)
	{
		//ROTATE X
		AglMatrix mat(1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1);
		mat = AglMatrix::inverse(mat);
		Scene::GetInstance()->Transform(mat);
	}
	else if (index == 4)
	{
		//ROTATE Y
		AglMatrix mat(0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1);
		mat = AglMatrix::inverse(mat);
		Scene::GetInstance()->Transform(mat);
	}
	else if (index == 5)
	{
		//ROTATE Z
		AglMatrix mat(0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		mat = AglMatrix::inverse(mat);
		Scene::GetInstance()->Transform(mat);
	}
}

void TW_CALL SceneDialog::LoadAGL(void *clientData)
{
	SceneDialog* sceneDialog = (SceneDialog*)clientData;
	string file = openfilename("Agile Files (*.agl*)\0*.agl*\0");
	if (file != "")
	{
		AGLLoader::GetInstance()->Load(file);

		vector<Mesh*> meshes = Scene::GetInstance()->GetMeshes();
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			string s = "Mesh" + toString(i+1);
			string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Meshes' label='" + meshes[i]->GetName() + "'";

			TwAddButton(sceneDialog->m_dialog, s.c_str(), OpenMeshDialog, (void*)i, info.c_str());
		}
		vector<AglMaterial*> materials = Scene::GetInstance()->GetMaterials();
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			string s = Scene::GetInstance()->GetName(materials[i]->nameID);
			string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Materials'";

			TwAddButton(sceneDialog->m_dialog, ("Material" + toString(materials[i]->id)).c_str(), OpenMaterialDialog, (void*)i, info.c_str());
		}

		TwAddButton(sceneDialog->m_dialog, "AddMaterial", AddMaterial, sceneDialog, " label='Material' key=c help='Load an Agile file into the editor.' group='Add'");
		TwAddButton(sceneDialog->m_dialog, "AddParticleEffect", AddPE, sceneDialog, " label='Particle Effect' key=c help='Load an Agile file into the editor.' group='Add'");


		TwAddButton(sceneDialog->m_dialog, "FLIPX", SetCOSystem, (void*)0, " label='Flip X' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "FLIPY", SetCOSystem, (void*)1, " label='Flip Y' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "FLIPZ", SetCOSystem, (void*)2, " label='Flip Z' key=c help='Load an Agile file into the editor.' group='Coordinate System'");

		TwAddButton(sceneDialog->m_dialog, "Rot90X", SetCOSystem, (void*)3, " label='Rotate 90 X' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "Rot90Y", SetCOSystem, (void*)4, " label='Rotate 90 Y' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "Rot90Z", SetCOSystem, (void*)5, " label='Rotate 90 Z' key=c help='Load an Agile file into the editor.' group='Coordinate System'");


		//TwAddButton(sceneDialog->m_dialog, "Sys1", SetCOSystem, (void*)3, " label='Up: Y Forward: Z L' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		//TwAddButton(sceneDialog->m_dialog, "Sys2", SetCOSystem, (void*)4, " label='Up: Z Forward: -X L' key=c help='Load an Agile file into the editor.' group='Coordinate System'");


		TwAddVarRW(sceneDialog->m_dialog, "Sphere", TW_TYPE_BOOLCPP, &DRAWDEBUGSPHERE, "group='Debug'");
		TwAddVarRW(sceneDialog->m_dialog, "Box", TW_TYPE_BOOLCPP, &DRAWDEBUGBOX, "group='Debug'");

		vector<AglConnectionPoint> cps = Scene::GetInstance()->getConnectionPoints();
		for (unsigned int i = 0; i < cps.size(); i++)
		{
			string name = Scene::GetInstance()->GetName(cps[i].nameID);
			TwAddButton(sceneDialog->m_dialog, name.c_str(), NULL, (void*)0, "group='Connection Points'");
		}
		vector<ParticleSystem*> ps = Scene::GetInstance()->GetParticleSystems();
		for (unsigned int i = 0; i < ps.size(); i++)
		{
			string s = "NoName";
			string info = " label='" + s + "' group='Particle Effects'";
			int zero = i;

			SceneDialog* sceneDialog = (SceneDialog*)clientData;
			TwAddButton(sceneDialog->m_dialog, ("Particle Effect" + toString(zero)).c_str(), OpenParticleSystemDialog, (void*)i, info.c_str());
		}
	}
}
void TW_CALL SceneDialog::SaveAGL(void *clientData)
{
	string file = savefilename();
	Scene::GetInstance()->Save(file);
}
void TW_CALL SceneDialog::AddMaterial(void *clientData)
{
	AglMaterial* mat = new AglMaterial();
	mat->nameID = Scene::GetInstance()->AddName("");
	Scene::GetInstance()->AddMaterial(mat, false, false);
	string s = Scene::GetInstance()->GetName(mat->nameID);
	string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Materials'";

	SceneDialog* sceneDialog = (SceneDialog*)clientData;
	TwAddButton(sceneDialog->m_dialog, ("Material" + toString(mat->id)).c_str(), OpenMaterialDialog, (void*)mat->id, info.c_str());
}
void TW_CALL SceneDialog::AddPE(void* clientData)
{
	AglParticleSystem* ps = new AglParticleSystem();
	Scene::GetInstance()->AddParticleSystem(ps);
	string s = "NoName";
	string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Particle Effects'";
	int zero = Scene::GetInstance()->GetParticleSystems().size()-1;

	SceneDialog* sceneDialog = (SceneDialog*)clientData;
	TwAddButton(sceneDialog->m_dialog, ("Particle Effect" + toString(zero)).c_str(), OpenParticleSystemDialog, (void*)zero, info.c_str());
}
void SceneDialog::ClonePE(AglParticleSystemHeader pHeader)
{
	AglParticleSystem* ps = new AglParticleSystem(pHeader);
	Scene::GetInstance()->AddParticleSystem(ps);
	string s = "NoName";
	string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Particle Effects'";
	int zero = Scene::GetInstance()->GetParticleSystems().size()-1;

	TwAddButton(this->m_dialog, ("Particle Effect" + toString(zero)).c_str(), OpenParticleSystemDialog, (void*)zero, info.c_str());
}

SceneDialog::SceneDialog()
{
	// Create a tweak bar
    m_dialog = TwNewBar("Scene");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	int barSize[2] = {200, 1060};
    TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);
	TwDefine(" Scene position='10 10' ");

	TwAddButton(m_dialog, "Load Scene", LoadAGL, this, " label='Load Scene' key=c help='Load an Agile file into the editor.' group='I/O'");
	TwAddButton(m_dialog, "Save Scene", SaveAGL, this, " label='Save Scene' key=c help='Save a scene into an Agile file.' group='I/O'");

	TwAddVarRW(m_dialog, "Rotation", TW_TYPE_QUAT4F, Scene::GetInstance()->GetQuaternionRotation(), "opened=true axisz=-z");

	TwStructMember pointMembers[] = { 
        { "X", TW_TYPE_FLOAT, offsetof(AglVector3, x), " Step=0.01 " },
        { "Y", TW_TYPE_FLOAT, offsetof(AglVector3, y), " Step=0.01 " },
		{ "Z", TW_TYPE_FLOAT, offsetof(AglVector3, z), " Step=0.01 " }};
    TwType pointType = TwDefineStruct("POINT", pointMembers, 3, sizeof(AglVector3), NULL, NULL);

	TwAddVarRW(m_dialog, "Position", pointType, Scene::GetInstance()->GetPosition(), "");

	TwAddVarRW(m_dialog, "ShowHideDiffuse", TW_TYPE_BOOLCPP, &DIFFUSEON, "group='Show/Hide'");
	TwAddVarRW(m_dialog, "ShowHideSpec", TW_TYPE_BOOLCPP, &SPECULARON, "group='Show/Hide'");
	TwAddVarRW(m_dialog, "ShowHideGlow", TW_TYPE_BOOLCPP, &GLOWON, "group='Show/Hide'");
	TwAddVarRW(m_dialog, "ShowHideNormal", TW_TYPE_BOOLCPP, &NORMALON, "group='Show/Hide'");

	m_meshDialog = new MeshDialog();
	m_materialDialog = new MaterialDialog();
	m_particleSystemDialog = new ParticleSystemDialog();
}
SceneDialog::~SceneDialog()
{
	delete m_meshDialog;
	delete m_materialDialog;
	delete m_particleSystemDialog;
}

SceneDialog* SceneDialog::GetInstance()
{
	if (!sInstance)
		sInstance = new SceneDialog();
	return sInstance;
}
void SceneDialog::Release()
{
	delete sInstance;
}
void SceneDialog::SetCurrentMesh(int pIndex)
{
	//m_materialDialog->hide();
	m_particleSystemDialog->hide();

	m_meshDialog->setMesh(pIndex);
}
void SceneDialog::SetCurrentParticleSystem(int pIndex)
{
	m_materialDialog->hide();
	m_meshDialog->hide();

	m_particleSystemDialog->setPS(pIndex);
}
void SceneDialog::SetCurrentMaterial(int pIndex)
{
	m_meshDialog->hide();
	m_particleSystemDialog->hide();

	m_materialDialog->setMaterial(pIndex);
}