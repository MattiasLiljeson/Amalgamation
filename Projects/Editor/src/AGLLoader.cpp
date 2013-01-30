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
	Scene::GetInstance()->Init(pPath, mDevice, mDeviceContext);
}
AGLLoader::~AGLLoader()
{
	if (mPath)
		delete[] mPath;

	if (mReader)
		delete mReader;
}