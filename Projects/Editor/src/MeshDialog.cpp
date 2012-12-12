#include "MeshDialog.h"
#include "Mesh.h"

string meshName;

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
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);
	const std::string *srcPtr = static_cast<const std::string *>(value);
	m->SetName(*srcPtr);

	string define = "Scene/Mesh" + toString(d->m_meshIndex+1) + " label ='" + *srcPtr + "'";
	TwDefine(define.c_str());
}
void TW_CALL MeshDialog::GetName(void *value, void *clientData)
{
	MeshDialog* d = (MeshDialog*)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(d->m_meshIndex);

	std::string *dest = (std::string *)(value);
	meshName = m->GetName();
	TwCopyStdStringToLibrary(*dest, meshName);
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

	TwAddVarCB(m_dialog, "MeshName", TW_TYPE_STDSTRING, SetName, GetName, (void*)this, " label='Name: '");
	TwAddVarCB(m_dialog, "Wireframe", TW_TYPE_BOOLCPP, SetWireframe, GetWireframe, (void*)this, "group=Sponge key=o");
	TwAddVarCB(m_dialog, "Normal Length", TW_TYPE_FLOAT, SetDrawNormals, GetDrawNormals, (void*)this, "min=0.0 max=1.0 step=0.001");
	TwAddVarCB(m_dialog, "Current Material", TW_TYPE_FLOAT, SetDrawNormals, GetDrawNormals, (void*)this, "min=0.0 max=1.0 step=0.001");
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