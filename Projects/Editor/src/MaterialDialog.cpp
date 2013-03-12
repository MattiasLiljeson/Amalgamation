#include "MaterialDialog.h"
#include "Scene.h"
#include "TextureManager.h"
#include <AglVector4.h>
#include "SceneDialog.h"

//Callbacks
void TW_CALL MaterialDialog::LoadDiffuse(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->diffuseTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
}
void TW_CALL MaterialDialog::LoadSpecular(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->specularTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
}
void TW_CALL MaterialDialog::LoadGlow(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->glowTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
}
void TW_CALL MaterialDialog::LoadNormal(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->normalTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
}
void TW_CALL MaterialDialog::LoadDisplacement(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->displacementTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
}
void TW_CALL MaterialDialog::LoadGradient(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;

	string file = openfilename();
	if (file != "")
	{
		mat->gradientTextureNameIndex = Scene::GetInstance()->AddName(file);
		TextureManager::GetInstance()->LoadTexture(file);
	}
	if (mat->gradientDataIndex < 0)
	{
		AddLayer(dialog);
	}
}
void TW_CALL MaterialDialog::AddLayer(void *clientData)
{
	MaterialDialog* dialog = (MaterialDialog*)clientData;
	AglMaterial* mat = dialog->m_material;
	AglGradient* g;
	AglGradientMaterial* layer = NULL;
	int index;
	if (mat->gradientDataIndex < 0)
	{
		g = new AglGradient();
		mat->gradientDataIndex = Scene::GetInstance()->AddGradient(g);
		layer = new AglGradientMaterial();
		g->addLayer(layer);
		layer = g->getLayers()[0];
		index = 0;
	}
	else
	{
		g = Scene::GetInstance()->GetGradient(mat->gradientDataIndex);
		layer = new AglGradientMaterial();
		g->addLayer(layer);
		index = g->getLayers().size()-1;
	}
	dialog->AddLayer(g, layer, index);
}
void MaterialDialog::AddLayer(AglGradient* pGradient, AglGradientMaterial* pLayer, int pIndex)
{
	string name = "Layer" + toString(pIndex);

	string define = "group='"+name+"'";
	string color = "Color" + toString(pIndex);
	AglVector4* c = pGradient->getLayerColorPointer(pIndex);
	TwAddVarRW(m_dialog, color.c_str(), TW_TYPE_COLOR4F, (void*)c, define.c_str());

	string remove = "Remove" + toString(pIndex);
	TwAddButton(m_dialog, remove.c_str(), RemoveGradientLayer, (void*)pLayer, define.c_str());

	define = "Material/'" + name + "' group='Gradient Mapping'";
	TwDefine(define.c_str());
}
void TW_CALL MaterialDialog::SetName(const void *value, void *clientData)
{
	const char *src = *(const char **)value;
	size_t srcLen = strlen(src);
	string res = "";
	for (unsigned int i = 0; i < srcLen; i++)
	{
		res += src[i];
	}

	MaterialDialog* d = (MaterialDialog*)clientData;
	AglMaterial* m = d->m_material;
	m->nameID = Scene::GetInstance()->AddName(res);

	string define = "Scene/Material" + toString(m->id) + " label ='" + res + "'";
	TwDefine(define.c_str());
}
void TW_CALL MaterialDialog::GetName(void *value, void *clientData)
{
	char **destPtr = (char **)value;
	MaterialDialog* d = (MaterialDialog*)clientData;
	AglMaterial* m = d->m_material;
	string materialName = Scene::GetInstance()->GetName(m->nameID);
	char *src = (char*)materialName.c_str();
	TwCopyCDStringToLibrary(destPtr, src);
}
void TW_CALL MaterialDialog::Delete(void *clientData)
{
	MaterialDialog* d = (MaterialDialog*)clientData;
	AglMaterial* m = d->m_material;
	d->hide();
	SceneDialog::GetInstance()->RemoveMaterial(m);
}
void TW_CALL MaterialDialog::RemoveGradientLayer(void *clientData)
{
	AglGradientMaterial* layer = (AglGradientMaterial*)clientData;
	vector<AglGradient*> gradients = Scene::GetInstance()->GetGradients();
	for (unsigned int i = 0; i < gradients.size(); i++)
	{
		vector<AglGradientMaterial*> layers = gradients[i]->getLayers();
		for (unsigned int j = 0; j < layers.size(); j++)
		{
			if (layers[j] == layer)
			{
				MaterialDialog* dialog = SceneDialog::GetInstance()->GetMaterialDialog();
				for (unsigned int l = 0; l < layers.size(); l++)
				{
					//Remove layers here				
					string name = "Layer" + toString(l);
					TwRemoveVar(dialog->m_dialog, name.c_str());
				}
				gradients[i]->removeLayer(layer);
				layers = gradients[i]->getLayers();
				for (unsigned int l = 0; l < layers.size(); l++)
				{
					//Add remaining layers
					dialog->AddLayer(gradients[i], layers[l], l);
				}

				break;
			}
		}
	}
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
	int barSize[2] = {200, 1060};
	TwDefine(" Material position='1710 10'  ");
	TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);

	m_material = Scene::GetInstance()->GetMaterial(pIndex);

	TwAddVarCB(m_dialog, "MaterialName", TW_TYPE_CDSTRING, SetName, GetName, (void*)this, " label='Name: '");
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
	TwAddButton(m_dialog, "Load Gradient Texture", LoadGradient, this, " label='Gradient Texture' key=c help='Load an Agile file into the editor.' group='Load'");

	TwStructMember tessMembers[] = { 
		{ "Edge1", TW_TYPE_FLOAT, offsetof(AglVector4, x), " Step=0.1 min=1.0 max=64.0" },
		{ "Edge2", TW_TYPE_FLOAT, offsetof(AglVector4, y), " Step=0.1 min=1.0 max=64.0" },
		{ "Edge3", TW_TYPE_FLOAT, offsetof(AglVector4, z), " Step=0.1 min=1.0 max=64.0" },
		{ "Internal", TW_TYPE_FLOAT, offsetof(AglVector4, w), " Step=0.1 min=1.0 max=64.0" }};
	TwType tessType = TwDefineStruct("TESSELATIONFACTORS", tessMembers, 4, sizeof(AglVector4), NULL, NULL);

	TwAddVarRW(m_dialog, "Tess", tessType, &m_material->tesselationFactor, " group='Properties' ");
	TwAddVarRW(m_dialog, "Displacement", TW_TYPE_FLOAT, (void*)&m_material->displacement, " group='Properties' min=0.0 max=10.0 step=0.01");

	TwAddButton(m_dialog, "Add Layer", AddLayer, this, " label='Add Layer' key=c help='Load an Agile file into the editor.' group='Gradient Mapping'");

	TwAddButton(m_dialog, "Delete", Delete, this, "");


	if (m_material->gradientDataIndex >= 0)
	{
		AglGradient* g = Scene::GetInstance()->GetGradient(m_material->gradientDataIndex);
		vector<AglGradientMaterial*> layers = g->getLayers();
		for (unsigned int i = 0; i < layers.size(); i++)
		{
			AddLayer(g, layers[i], i);
		}
	}


	show();
}