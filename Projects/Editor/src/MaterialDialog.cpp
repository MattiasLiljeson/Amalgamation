#include "MaterialDialog.h"
#include "Scene.h"
#include "TextureManager.h"
#include <AglVector4.h>

string materialName;

//Callbacks
void TW_CALL MaterialDialog::LoadDiffuse(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		mat->diffuseTextureNameIndex = Scene::GetInstance()->AddName(file);
	}
}
void TW_CALL MaterialDialog::LoadSpecular(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		mat->specularTextureNameIndex = Scene::GetInstance()->AddName(file);
	}
}
void TW_CALL MaterialDialog::LoadGlow(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		mat->glowTextureNameIndex = Scene::GetInstance()->AddName(file);
	}
}
void TW_CALL MaterialDialog::LoadNormal(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		mat->normalTextureNameIndex = Scene::GetInstance()->AddName(file);
	}
}
void TW_CALL MaterialDialog::LoadDisplacement(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		removePath(file);
		file = Scene::GetInstance()->GetFolder() + file;
		TextureManager::GetInstance()->LoadTexture(file);
		mat->displacementTextureNameIndex = Scene::GetInstance()->AddName(file);
	}
}
void TW_CALL MaterialDialog::SetName(const void *value, void *clientData)
{
	MaterialDialog* d = (MaterialDialog*)clientData;
	AglMaterial* m = d->m_material;
	const std::string *srcPtr = static_cast<const std::string *>(value);
	m->nameID = Scene::GetInstance()->AddName(*srcPtr);

	string define = "Scene/Material" + toString(m->id) + " label ='" + *srcPtr + "'";
	TwDefine(define.c_str());
}
void TW_CALL MaterialDialog::GetName(void *value, void *clientData)
{
	MaterialDialog* d = (MaterialDialog*)clientData;
	AglMaterial* m = d->m_material;

	std::string *dest = (std::string *)(value);
	string hej = *dest;
	materialName = Scene::GetInstance()->GetName(m->nameID);
	TwCopyStdStringToLibrary(*dest, materialName);
}

MaterialDialog::MaterialDialog()
{
	m_material = NULL;
	hide();
}
MaterialDialog::~MaterialDialog()
{
}
void MaterialDialog::show()
{
	TwDefine(" Material visible=true ");
}
void MaterialDialog::hide()
{
	TwDefine(" Material visible=false ");
}
void MaterialDialog::setMaterial(int pIndex)
{
	if (m_dialog)
		TwDeleteBar(m_dialog);
	// Create a tweak bar
	m_dialog = TwNewBar("Material");
	int barSize[2] = {200, 300};
	TwDefine(" Material position='590 320' ");
	TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);

	m_material = Scene::GetInstance()->GetMaterial(pIndex);

	TwAddVarCB(m_dialog, "MaterialName", TW_TYPE_STDSTRING, SetName, GetName, (void*)this, " label='Name: '");
	TwAddVarRW(m_dialog, "Ambient", TW_TYPE_COLOR3F, (void*)&m_material->ambient, " help='Light color.' group='Properties'");
	TwAddVarRW(m_dialog, "Diffuse", TW_TYPE_COLOR3F, (void*)&m_material->diffuse, " help='Light color.' group='Properties'");
	TwAddVarRW(m_dialog, "Specular", TW_TYPE_COLOR3F, (void*)&m_material->specular, " help='Light color.' group='Properties'");
	TwAddVarRW(m_dialog, "Emissive", TW_TYPE_COLOR3F, (void*)&m_material->emissive, " help='Light color.' group='Properties'");

	TwAddVarRW(m_dialog, "Opacity", TW_TYPE_FLOAT, (void*)&m_material->opacity, " help='Light color.' group='Properties' min=0.0 max=1.0 step=0.01");
	TwAddVarRW(m_dialog, "Reflectivity", TW_TYPE_FLOAT, (void*)&m_material->reflectivity, " help='Light color.' group='Properties' min=0.0 max=1.0 step=0.01");
	TwAddVarRW(m_dialog, "Shininess", TW_TYPE_FLOAT, (void*)&m_material->shininess, " help='Light color.' group='Properties' min=0.0 max=100.0 step=1.0");
	TwAddVarRW(m_dialog, "Texture Scale", TW_TYPE_FLOAT, (void*)&m_material->textureScale, " help='Light color.' group='Properties' min=0.0 max=10.0 step=0.01");

	TwAddButton(m_dialog, "Load Diffuse Texture", LoadDiffuse, this, " label='Diffuse Texture' key=c help='Load an Agile file into the editor.' group='Load'");
	TwAddButton(m_dialog, "Load Specular Texture", LoadSpecular, this, " label='Specular Texture' key=c help='Load an Agile file into the editor.' group='Load'");
	TwAddButton(m_dialog, "Load Glow Texture", LoadGlow, this, " label='Glow Texture' key=c help='Load an Agile file into the editor.' group='Load'");
	TwAddButton(m_dialog, "Load Normal Texture", LoadNormal, this, " label='Normal Texture' key=c help='Load an Agile file into the editor.' group='Load'");
	TwAddButton(m_dialog, "Load Displacement Texture", LoadDisplacement, this, " label='Displacement Texture' key=c help='Load an Agile file into the editor.' group='Load'");

	TwStructMember tessMembers[] = { 
		{ "Edge1", TW_TYPE_FLOAT, offsetof(AglVector4, x), " Step=0.1 min=1.0 max=64.0" },
		{ "Edge2", TW_TYPE_FLOAT, offsetof(AglVector4, y), " Step=0.1 min=1.0 max=64.0" },
		{ "Edge3", TW_TYPE_FLOAT, offsetof(AglVector4, z), " Step=0.1 min=1.0 max=64.0" },
		{ "Internal", TW_TYPE_FLOAT, offsetof(AglVector4, w), " Step=0.1 min=1.0 max=64.0" }};
	TwType tessType = TwDefineStruct("TESSELATIONFACTORS", tessMembers, 4, sizeof(AglVector4), NULL, NULL);

	TwAddVarRW(m_dialog, "Tess", tessType, &m_material->tesselationFactor, " group='Properties' ");
	TwAddVarRW(m_dialog, "Displacement", TW_TYPE_FLOAT, (void*)&m_material->displacement, " group='Properties' min=0.0 max=10.0 step=0.01");

	show();
}