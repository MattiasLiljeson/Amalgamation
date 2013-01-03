#include "AGLLoader.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "SkeletonMapping.h"
#include "SkeletonMesh.h"

AGLLoader* AGLLoader::sInstance = NULL;

AGLLoader::AGLLoader()
{
	mPath = NULL;
	mReader = NULL;
}

AGLLoader* AGLLoader::GetInstance()
{
	if (!sInstance)
		sInstance = new AGLLoader();
	return sInstance;
}
void AGLLoader::Release()
{
	if (sInstance)
		delete sInstance;
	sInstance = NULL;
}
void AGLLoader::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
}
void AGLLoader::Load(string pPath)
{
	mPath = new char[pPath.length()+1];
	const char* src = pPath.c_str();
	strcpy_s(mPath, pPath.length()+1, src);
	mReader = new AglReader(mPath);

	vector<AglMesh*> meshes = mReader->getScene()->getMeshes();
	vector<Mesh*> modmeshes;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Mesh* m = new Mesh(mDevice, mDeviceContext, Scene::GetInstance());
		m->Init(meshes[i], mReader);
		modmeshes.push_back(m);
	}

	vector<AglSkeletonMapping*> mappings = mReader->getScene()->getSkeletonMappings();
	vector<SkeletonMapping*> modmappings;
	for (unsigned int i = 0; i < mappings.size(); i++)
	{
		SkeletonMapping* sm = new SkeletonMapping(mDevice, mappings[i]);
		modmappings.push_back(sm);
	}

	vector<AglMaterial*> materials = mReader->getScene()->getMaterials();
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		string path = pPath;
		int ind = path.find_last_of('\\');
		if (ind == -1)
			ind = path.find_last_of('/');
		path = path.substr(0, ind+1);	

		if (materials[i]->diffuseTextureNameIndex >= 0)
		{
			string difpath = mReader->getScene()->getName(materials[i]->diffuseTextureNameIndex);
			removePath(difpath);
			difpath = path + difpath;
			TextureManager::GetInstance()->LoadTexture(difpath);
		}
		if (materials[i]->specularTextureNameIndex >= 0)
		{
			string specpath = mReader->getScene()->getName(materials[i]->specularTextureNameIndex);
			removePath(specpath);
			specpath = path + specpath;
			TextureManager::GetInstance()->LoadTexture(specpath);
		}
		if (materials[i]->normalTextureNameIndex >= 0)
		{
			string normpath = mReader->getScene()->getName(materials[i]->normalTextureNameIndex);
			removePath(normpath);
			normpath = path + normpath;
			TextureManager::GetInstance()->LoadTexture(normpath);
		}
		if (materials[i]->glowTextureNameIndex >= 0)
		{
			string glowpath = mReader->getScene()->getName(materials[i]->glowTextureNameIndex);
			removePath(glowpath);
			glowpath = path + glowpath;
			TextureManager::GetInstance()->LoadTexture(glowpath);
		}
	}

	vector<AglMaterialMapping> mm = mReader->getScene()->getMaterialMappings();
	for (unsigned int i = 0; i < mm.size(); i++)
	{
		modmeshes[mm[i].meshID]->AddMaterial(mm[i].materialID, true);
	}

	string folder = pPath;
	int ind = folder.find_last_of('\\');
	if (ind == -1)
		ind = folder.find_last_of('/');
	folder = folder.substr(0, ind+1);

	vector<SkeletonMesh*> skeletons;
	for (unsigned int i = 0; i < mReader->getScene()->getSkeletonCount(); i++)
	{
		skeletons.push_back(new SkeletonMesh(mDevice, mDeviceContext, mReader->getScene()->getSkeleton(i)));
	}

	vector<AglLooseBspTree*> bspTrees = mReader->getScene()->getBspTrees();
	for (unsigned int i = 0; i < bspTrees.size(); i++)
	{
		modmeshes[bspTrees[i]->getHeader().targetMesh]->SetBspTree(bspTrees[i]);
	}
	vector<AglInteriorSphereGrid*> grids = mReader->getScene()->getSphereGrids();
	for (unsigned int i = 0; i < grids.size(); i++)
	{
		modmeshes[grids[i]->getHeader().targetMesh]->SetInteriorSpheres(grids[i]);
	}


	Scene::GetInstance()->Init(modmeshes, skeletons, modmappings, mReader->getScene(), folder, mDevice, mDeviceContext);
}
AGLLoader::~AGLLoader()
{
	if (mPath)
		delete[] mPath;

	if (mReader)
		delete mReader;
}