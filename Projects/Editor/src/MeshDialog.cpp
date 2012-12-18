#include "MeshDialog.h"
#include "Mesh.h"

void TW_CALL MeshDialog::SetWireframe(const void *value, void *clientData)
{
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	m->SetWireframe(*(const bool*)value);
}
void TW_CALL MeshDialog::GetWireframe(void *value, void *clientData)
{ 
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	*(bool*)value = m->GetWireframe();
}
void TW_CALL MeshDialog::SetDrawNormals(const void *value, void *clientData)
{
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	m->SetDrawNormals(*(const float*)value);
}
void TW_CALL MeshDialog::GetDrawNormals(void *value, void *clientData)
{
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	*(float*)value = m->GetDrawNormals();
}
void TW_CALL MeshDialog::SetName(const void *value, void *clientData)
{
	const char *src = *(const char **)value;
	size_t srcLen = strlen(src);
	string res = "";
	for (unsigned int i = 0; i < srcLen; i++)
	{
		res += src[i];
	}

	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	m->SetName(res);

	string define = "Scene/Mesh" + toString(d->m_meshIndex+1) + " label ='" + res + "'";
	TwDefine(define.c_str());
}
void TW_CALL MeshDialog::GetName(void *value, void *clientData)
{
	char **destPtr = (char **)value;
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	string meshName = m->GetName();
	char *src = (char*)meshName.c_str();
	TwCopyCDStringToLibrary(destPtr, src);
}
void TW_CALL MeshDialog::CreateSphereGrid(void *clientData)
{
	int id = (int)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(id);
	m->createSphereGrid();
}
void TW_CALL MeshDialog::CreateBspTree(void *clientData)
{
	int id = (int)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(id);
	m->createBspTree();
}

MeshDialog::MeshDialog()
{
	// Create a tweak bar
    m_dialog = NULL;
    int barSize[2] = {200, 300};
	TwDefine(" Mesh position='590 10' ");
    TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);
	hide();
	m_meshIndex = -1;
}
MeshDialog::~MeshDialog()
{
}
void MeshDialog::show()
{
	if (m_dialog)
		TwDeleteBar(m_dialog);

	m_dialog = TwNewBar("Mesh");
    int barSize[2] = {200, 300};
	TwDefine(" Mesh position='590 10' ");
    TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);
	
	Mesh* m = Scene::GetInstance()->GetMesh(m_meshIndex);

	TwAddVarCB(m_dialog, "MeshName", TW_TYPE_CDSTRING, SetName, GetName, (void*)this, " label='Name: '");
	TwAddVarCB(m_dialog, "Wireframe", TW_TYPE_BOOLCPP, SetWireframe, GetWireframe, (void*)this, "group=Sponge key=o");
	TwAddVarCB(m_dialog, "Normal Length", TW_TYPE_FLOAT, SetDrawNormals, GetDrawNormals, (void*)this, "min=0.0 max=1.0 step=0.001");
	TwAddVarCB(m_dialog, "Current Material", TW_TYPE_FLOAT, SetDrawNormals, GetDrawNormals, (void*)this, "min=0.0 max=1.0 step=0.001");
	
	TwAddButton(m_dialog, "CreateSphereGrid", CreateSphereGrid, (void*)m_meshIndex, " label='Create' group='Sphere Grid'");
	TwAddVarRW(m_dialog, "DrawSphereGrid", TW_TYPE_BOOLCPP, (void*)m->getDrawGrid(), "group='Sphere Grid'");

	TwAddButton(m_dialog, "CreateBspTree", CreateBspTree, (void*)m_meshIndex, " label='Create' group='Bsp Tree'");
	TwAddVarRW(m_dialog, "DrawBspTree", TW_TYPE_BOOLCPP, (void*)m->getDrawTree(), "group='Bsp Tree'");
	TwAddVarRW(m_dialog, "BspTreeLevel", TW_TYPE_UINT32, (void*)m->getTreeLevel(), "group='Bsp Tree'");
	TwDefine(" Mesh visible=true ");
}
void MeshDialog::hide()
{
	TwDefine(" Mesh visible=false ");
}
void MeshDialog::setMesh(int pIndex)
{
	m_meshIndex = pIndex;
	show();
}