#include "SceneDialog.h"

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

void TW_CALL SceneDialog::LoadAGL(void *clientData)
{
	SceneDialog* sceneDialog = (SceneDialog*)clientData;
	string file = openfilename("Agile Files (*.agl*)\0*.agl*\0");
	if (file != "")
	{
		AGLLoader::GetInstance()->Load(file);

		vector<Mesh*> meshes = Scene::GetInstance()->GetMeshes();
		for (int i = 0; i < meshes.size(); i++)
		{
			string s = "Mesh" + toString(i+1);
			string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Meshes' label='" + meshes[i]->GetName() + "'";

			TwAddButton(sceneDialog->m_dialog, s.c_str(), OpenMeshDialog, (void*)i, info.c_str());
		}
		vector<AglMaterial*> materials = Scene::GetInstance()->GetMaterials();
		for (int i = 0; i < materials.size(); i++)
		{
			string s = Scene::GetInstance()->GetName(materials[i]->nameID);
			string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Materials'";

			TwAddButton(sceneDialog->m_dialog, ("Material" + toString(materials[i]->id)).c_str(), OpenMaterialDialog, (void*)i, info.c_str());
		}

		TwAddButton(sceneDialog->m_dialog, "AddMaterial", AddMaterial, sceneDialog, " label='Material' key=c help='Load an Agile file into the editor.' group='Add'");
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