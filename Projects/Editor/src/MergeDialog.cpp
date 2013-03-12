#include "MergeDialog.h"
#include "AglReader.h"
#include "SceneDialog.h"
#include "TextureManager.h"

void TW_CALL MergeDialog::LoadAGL(void *clientData)
{
	MergeDialog* dialog = (MergeDialog*)clientData;
	string file = openfilename("Agile Files (*.agl*)\0*.agl*\0");
	if (file != "")
	{
		AglReader* reader = new AglReader(file.c_str());
		AglScene* scene = reader->getScene();
		if (dialog->m_importParticleSystems)
		{
			vector<AglParticleSystem*> ps = scene->getParticleSystems();
			for (unsigned int i = 0; i < ps.size(); i++)
			{
				AglParticleSystemHeader h = ps[i]->getHeader();
				if (h.textureNameIndex >= 0)
				{
					string texPath = scene->getName(h.textureNameIndex);
					int ind = TextureManager::GetInstance()->LoadTexture(texPath);
					if (ind < 0)
					{
						h.textureNameIndex = -1;
					}
					else
					{
						TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
						h.textureNameIndex = Scene::GetInstance()->AddName(data->Path);
					}
				}
				SceneDialog::GetInstance()->ClonePE(h);
			}
		}
		delete scene;
		delete reader;
	}
}
MergeDialog::MergeDialog()
{
	// Create a tweak bar
	m_dialog = TwNewBar("Merge");
    int barSize[2] = {200, 1060};
	TwDefine(" Merge position='1710 10'  ");
	TwSetParam(m_dialog, NULL, "size", TW_PARAM_INT32, 2, barSize);
	hide();

	m_importMeshes = true;
	m_importParticleSystems = true;
	m_importMaterials = true;
	m_importAnimations = true;

	//TwAddVarRW(m_dialog, "Import Meshes", TW_TYPE_BOOLCPP, (void*)&m_importMeshes, "");
	TwAddVarRW(m_dialog, "Import Particle Systems", TW_TYPE_BOOLCPP, (void*)&m_importParticleSystems, "");
	//TwAddVarRW(m_dialog, "Import Materials", TW_TYPE_BOOLCPP, (void*)&m_importMaterials, "");
	//TwAddVarRW(m_dialog, "Import Animations", TW_TYPE_BOOLCPP, (void*)&m_importAnimations, "");

	TwAddButton(m_dialog, "Load Scene", LoadAGL, this, " label='Load Scene'");
}
MergeDialog::~MergeDialog()
{
}
void MergeDialog::show()
{
	TwDefine(" Merge visible=true ");
}
void MergeDialog::hide()
{
	TwDefine(" Merge visible=false ");
}