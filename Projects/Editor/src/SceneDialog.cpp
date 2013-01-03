#include "SceneDialog.h"
#include "Globals.h"

SceneDialog* SceneDialog::sInstance = NULL;

void TW_CALL SceneDialog::OpenMeshDialog(void *clientData)
{
	int index = (int)clientData;
	SceneDialog::GetInstance()->SetCurrentMesh(index);
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
		Scene::GetInstance()->SetCoordinateSystem(AglCoordinateSystem::DX());
	else if (index == 1)
		Scene::GetInstance()->SetCoordinateSystem(AglCoordinateSystem::GL());
	else if (index == 2)
		Scene::GetInstance()->SetCoordinateSystem(AglCoordinateSystem::BLENDER());
	else if (index == 3)
		Scene::GetInstance()->SetCoordinateSystem(AglCoordinateSystem(AglVector3(0, 1, 0), AglVector3(0, 0, 1), AglCoordinateSystem::LEFT));
	else if (index == 4)
		Scene::GetInstance()->SetCoordinateSystem(AglCoordinateSystem(AglVector3(0, 0, 1), AglVector3(-1, 0, 0), AglCoordinateSystem::LEFT));
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


		TwAddButton(sceneDialog->m_dialog, "DirectXSystem", SetCOSystem, (void*)0, " label='DirectX' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "OpenGLSystem", SetCOSystem, (void*)1, " label='OpenGL' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "BlenderSystem", SetCOSystem, (void*)2, " label='Blender' key=c help='Load an Agile file into the editor.' group='Coordinate System'");


		TwAddButton(sceneDialog->m_dialog, "Sys1", SetCOSystem, (void*)3, " label='Up: Y Forward: Z L' key=c help='Load an Agile file into the editor.' group='Coordinate System'");
		TwAddButton(sceneDialog->m_dialog, "Sys2", SetCOSystem, (void*)4, " label='Up: Z Forward: -X L' key=c help='Load an Agile file into the editor.' group='Coordinate System'");


		TwAddVarRW(sceneDialog->m_dialog, "Sphere", TW_TYPE_BOOLCPP, &DRAWDEBUGSPHERE, "group='Debug'");
		TwAddVarRW(sceneDialog->m_dialog, "Box", TW_TYPE_BOOLCPP, &DRAWDEBUGBOX, "group='Debug'");
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
	/*AglMaterial* mat = new AglMaterial();
	mat->nameID = Scene::GetInstance()->AddName("");
	Scene::GetInstance()->AddMaterial(mat, false, false);
	string s = Scene::GetInstance()->GetName(mat->nameID);*/
	string s = "NoName";
	string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Particle Effects'";
	int zero = 0;

	SceneDialog* sceneDialog = (SceneDialog*)clientData;
	TwAddButton(sceneDialog->m_dialog, ("Particle Effect" + toString(zero)).c_str(), OpenMaterialDialog, (void*)0, info.c_str());
}

SceneDialog::SceneDialog()
{
	// Create a tweak bar
    m_dialog = TwNewBar("Scene");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
    int barSize[2] = {200, 400};
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
}
SceneDialog::~SceneDialog()
{
	delete m_meshDialog;
	delete m_materialDialog;
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
	m_meshDialog->setMesh(pIndex);
}
void SceneDialog::SetCurrentMaterial(int pIndex)
{
	m_materialDialog->setMaterial(pIndex);
}